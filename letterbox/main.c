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
#include "storage.h"
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
void lb_read_message(void);
bool lb_save_message(void);
void lb_set_new_as_current(void);

/** refers to the currently selected message that is to be 
 * displayed on the LCD */
unsigned int current_message = 0;
bool new_message = false;

extern char global_buffer[];

char CONNECTION[] PROGMEM =
        "                     "
        "                     "
        "                     "
        " Message incoming... "
        "                     "
        "                     "
        "                     "
        "                     ";

char NOTICE[] PROGMEM  = 
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
    char* c_addr = CONNECTION;
    char c;
    lcd_clear();
    while ((c = pgm_read_byte(c_addr)) != '\0') {
        lcd_display_char_masked(c, 0xff);
        c_addr++;
    }
}

/** A screen panel */
void lb_display_notice(void) {
    char* c_addr = NOTICE;
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
    lcd_clear();
   
    if (storage_message_count() > 0) {
        // Load and display message
        storage_load_message(current_message);
        lcd_display_string(storage_get_buffer());
    }

    // Draw menu
    lcd_set_page(0);
    lcd_set_column(LCD_INIT_COLUMN);
    lcd_display_string_masked("Down", 0xff);
    lcd_display_string(" ");
    lcd_display_string_masked(" Up ", 0xff);
    // snprintf(global_buffer, 8, "   %d/%d  ", current_message, storage_message_count());
    // lcd_display_string(global_buffer);
    lcd_display_string("      ");
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


bool lb_save_message(void) {
    return storage_save_message();
}

void lb_check_connection(void)
{
    char* msg = bt_readline();
    if (lb_is_connect(msg)) {
        lb_display_connection();
        lb_read_message();
        lb_save_message();
        lb_set_new_as_current();
        new_message = true;
        lb_display_notice();
    }
}
 
void lb_read_message(void) {
    char* msg;
    int r = 0, s = 0;
    while (true) {
       msg = bt_readline();
       if (msg != NULL) {
            if (lb_is_disconnect(msg)) {
                led_blink();
                led_blink();
                led_blink();
                break;
            } else {
                lcd_display_string(msg);
                // TODO: could use streaming here,
                // but needs rewrite of storage module
                r = strlen(msg);
                memcpy(global_buffer + s, msg, r); 
                s += r;
            }
       }
    }   
}

void lb_set_new_as_current(void) {
    current_message = storage_message_count() - 1;
}

void lb_check_user_request(void)
{
    int key = get_key();
    if (!new_message) {
        switch (key)
        { // TODO: get rid of magic numbers
            case 4:
                led_on();
                // Down
                current_message = (current_message - 1) % storage_message_count();
                break;
            case 6:
                led_on();
                // Up 
                current_message = (current_message + 1) % storage_message_count();
                break;
            case 9:
                led_on();
                // Delete 
                storage_delete_message(current_message);
                current_message = (current_message - 1) % storage_message_count();
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
    // strcpy(storage_get_buffer(), "Message 1");
    // storage_save_message();
    // lb_display_message();
    // lb_display_connection();
    // _delay_ms(6000);
    // lb_display_notice();
    lb_serve();
    return 0;
}

