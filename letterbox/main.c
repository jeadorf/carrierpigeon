/** Main routine of the letterbox server. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "global.h"
#include "uart.h"
#include "lcd.h"
#include "led.h"
#include "text.h"
#include "bt.h"
#include "message.h"
#include "assert.h"
#include "buttons.h"
#include "timer.h"

/* This is the GOD main loop that is responsible for the control flow,
 * for toggling between the different panels of the user interface
 * for establishing connections to a bluetooth client, downloading
 * and saving messages.
 */

#define BUTTON_UP 6
#define BUTTON_DOWN 4
#define BUTTON_DELETE 9

void lb_init(void);
void lb_serve(void);
void lb_check_user_request(void);
void lb_check_connection(void);
bool lb_is_connect(char* msg);
bool lb_is_disconnect(char* msg);
void lb_force_disconnect(void);
void lb_display_message(void);
void lb_display_dialog(const char* pgm_msg);
void lb_capture_message(void);
void lb_set_new_as_current(void);

/** refers to the currently selected message that is to be 
 * displayed on the LCD */
unsigned int current_message = 0;
bool new_message = false;
#define READ_BUFFER_SIZE 32 
char read_buffer[READ_BUFFER_SIZE];

const char CONNECTION[] PROGMEM =
        "                     "
        "                     "
        "                     "
        " Message incoming... "
        "                     "
        "                     "
        "                     "
        "                     ";

const char NOTICE[] PROGMEM  = 
        "                     "
        "                     "
        "                     "
        "    Hey, look.       "
        "    You got mail!    "
        "                     "
        "                     "
        "    press any key    ";

const char FAIL[] PROGMEM  = 
        "                     "
        "                     "
        "                     "
        "     FAIL!           "
        "                     "
        "                     "
        "                     "
        "                     ";


const char MESSAGE_FULL[] PROGMEM  = 
        "                     "
        "                     "
        "                     "
        "  There is no space  "
        "  left for any new   "
        "  messages!          "
        "                     "
        "                     "
        "    press any key    ";

/** Initializes all components required by the letterbox. */
void lb_init(void)
{
    led_init();
    led_blink(LED_LINE_BLUE);
    led_blink(LED_LINE_RED);

    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, 11059200UL));
    sei();

    lcd_init();
    lcd_clear();
        
    timer_init();

    lb_display_message();
}

void lb_display_dialog(const char* pgm_msg)
{
    const char* c_addr = pgm_msg;
    char c;
    lcd_clear();
    while ((c = pgm_read_byte(c_addr)) != '\0') {
        lcd_display_char_masked(c, 0xff);
        c_addr++;
    }
    
}

/** A screen panel. Only call this method if the currently selected
 * message changed or if the LCD must switch to the message screen panel */
void lb_display_message(void) {
    char c;

    lcd_clear();
   
    if (message_empty()) {
        lcd_display_string_masked("There are no messages", 0xff);
    } else {
        // Load and display message
        message_open(current_message);
        while ((c = message_read()) != '\0') {
            lcd_display_char(c);
        }
        message_close();
    }

    // Draw menu
    lcd_set_page(0);
    lcd_set_column(LCD_INIT_COLUMN);
    lcd_display_string_masked("Down", 0xff);
    lcd_display_string(" ");
    lcd_display_string_masked(" Up ", 0xff);
    if (message_empty()) {
        lcd_display_string("  0/0 ");
    } else {
        snprintf(read_buffer, 7, "  %d/%d  ", current_message + 1, message_count());
        lcd_display_string(read_buffer);
    }
    lcd_display_string_masked(" Del. ", 0xff);
}

bool lb_is_connect(char* msg)
{
    return msg != NULL && strncmp(msg, "CONNECT  ", 9) == 0;
}

bool lb_is_disconnect(char* msg)
{
    return msg != NULL && strncmp(msg, "DISCONNECT  ", 12) == 0;
}

void lb_force_disconnect(void)
{
    uart_puts("+++\r\n");
    _delay_ms(100);
    uart_puts("ATH0\r\n");
    uart_puts("ATH1\r\n");
}

void lb_check_connection(void)
{
    bool read = bt_readline_buffer(read_buffer, READ_BUFFER_SIZE);
    if (read && lb_is_connect(read_buffer)) {
        if (message_full()) {
            lb_display_dialog(MESSAGE_FULL);
        } else {
            lb_display_dialog(CONNECTION);
            lb_capture_message();
            // Do not wait for the client to disconnect such that the
            // letterbox cannot neither be blocked by a client nor
            // miss the disconnect signal.
            lb_force_disconnect();
        }
    }
}
 
void lb_capture_message(void) {
    bool timeout = false;
    timer_start(15);
    
    // Open new record
    message_new();
    // Pipe next line directly to EEPROM
    while (!bt_readline_message(MESSAGE_TEXT_LENGTH) && !timeout) {
        timeout = timer_poll();
    }
    message_close();
    
    if (timeout) {
        // Delete the message which may have partly
        // written to the EEPROM
        message_delete(message_count() - 1);
        lb_display_dialog(FAIL);
        led_blink_times(LED_LINE_RED, 10);
        lb_display_message();
    } else {
        lb_set_new_as_current();
        new_message = true;
        led_on(LED_BLUE);
        lb_display_dialog(NOTICE);
    }
}

void lb_set_new_as_current(void) {
    current_message = message_count() - 1;
}

void lb_check_user_request(void)
{
    int key = get_key();
    if (!new_message) {
        switch (key)
        { 
            case BUTTON_DOWN:
                led_on(LED_GREEN);
                // Down
                if (current_message == 0) {
                    current_message = message_count() - 1;
                } else {
                    current_message = current_message - 1;
                }
                break;
            case BUTTON_UP:
                led_on(LED_GREEN);
                // Up 
                current_message = (current_message + 1) % message_count();
                break;
            case BUTTON_DELETE:
                led_on(LED_GREEN);
                // Delete 
                if (!message_empty()) {
                    message_delete(current_message);
                    current_message = (current_message - 1) % message_count();
                }
                break;
        }
    }

    if (key) {
        lb_display_message();
        new_message = false; 
        led_off(LED_BLUE);

        // wait until button is released
        while (get_key() != 0) {
            /* NOP */
        }
    }

    led_off(LED_GREEN);
}

/**
 * Lifecycle of the letterbox.
 */
void lb_serve(void)
{
    while (true) {
        lb_check_connection();
        lb_check_user_request();
    }
}

/** Letterbox life cycle. */
int main(void)
{
    lb_init();
    lb_serve();
    return 0;
}

