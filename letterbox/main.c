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

/* This is the GOD main loop that is responsible for the control flow,
 * for toggling between the different panels of the user interface
 * for establishing connections to a bluetooth client, downloading
 * and saving messages.
 */

void lb_init(void);
void lb_serve(void);
void lb_check_user_request(void);
void lb_check_connection(void);
bool lb_is_connect(char* msg);
bool lb_is_disconnect(char* msg);
void lb_display_connection(void);
void lb_display_notice(void);
void lb_display_message(void);
void lb_display_dialog(const char* pgm_msg);
void lb_capture_message(void);
void lb_read_disconnect(void);
void lb_set_new_as_current(void);

/** refers to the currently selected message that is to be 
 * displayed on the LCD */
unsigned int current_message = 0;
bool new_message = false;

extern char global_buffer[];

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
        "                     ";

/** Initializes all components required by the letterbox. */
void lb_init(void)
{
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, 11059200UL));
    sei();

    lcd_init();
    lcd_clear();
        
    lb_display_message();
}

/** A screen panel */
void lb_display_connection(void) {
    lb_display_dialog(CONNECTION);
}

/** A screen panel */
void lb_display_notice(void) {
    lb_display_dialog(NOTICE);
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
        while ((c = message_read())) {
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
        snprintf(global_buffer, 7, "  %d/%d  ", current_message + 1, message_count());
        lcd_display_string(global_buffer);
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

void lb_check_connection(void)
{
    bool read = bt_readline_buffer(global_buffer, MESSAGE_TEXT_LENGTH);
    if (read && lb_is_connect(global_buffer)) {
        lb_display_connection();
        lb_capture_message();
        lb_read_disconnect();
        lb_set_new_as_current();
        new_message = true;
        lb_display_notice();
    }
}
 
void lb_capture_message(void) {
    // open new record
    message_new();
    // pipe next line directly to EEPROM
    while (!bt_readline_message(MESSAGE_TEXT_LENGTH)) {
        // Wait for message 
    }
    message_close();
}

void lb_read_disconnect(void)
{
    while (!bt_readline_buffer(global_buffer, MESSAGE_TEXT_LENGTH)) {
        // wait until disconnect arrives
    }
    if (lb_is_disconnect(global_buffer)) {
        led_blink();
    } else {
        lcd_display_string("error in communication");
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
            // TODO: get rid of magic numbers
            // FIXME: % modulo operator does nonsense when applied to negative numbers (but why?)
            case 4:
                led_on();
                // Down
                if (current_message == 0) {
                    current_message = message_count() - 1;
                } else {
                    current_message = current_message - 1;
                }
                break;
            case 6:
                led_on();
                // Up 
                current_message = (current_message + 1) % message_count();
                break;
            case 9:
                led_on();
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
    }

    // wait until button is released
    while (get_key() != 0) {
        /* NOP */
    }

    led_off();
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

