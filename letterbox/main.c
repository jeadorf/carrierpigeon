/** Main routine of the letterbox server. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "global.h"
#include "uart.h"
#include "lcd.h"
#include "led.h"
#include "text.h"
#include "bt.h"
#include "storage.h"
#include "assert.h"
#include "buttons.h"

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

unsigned int current_message = 0;

/**
 * Initializes all components required by the letterbox.
 */
void lb_init(void)
{
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, 11059200UL));
    sei();

    lcd_init();
    lcd_clear();
        
    lb_display_message();
}

bool lb_is_connect(char* msg)
{
    return msg != NULL && strncmp(msg, "CONNECT  ", 9) == 0;
}

bool lb_is_disconnect(char* msg)
{
    return msg != NULL && strncmp(msg, "DISCONNECT  ", 12) == 0;
}

void lb_display_connection(void) {
}

void lb_display_notice(void) {
}

void lb_display_message(void) {
    storage_load_message(current_message);
    lcd_clear();
    lcd_display_string(storage_get_buffer());
    lcd_set_page(0);
    lcd_set_column(LCD_INIT_COLUMN);
    lcd_display_string_masked("Down", 0xff);
    lcd_display_string(" ");
    lcd_display_string_masked(" Up ", 0xff);
    lcd_display_string("        ");
    lcd_display_string_masked("Del.", 0xff);
}

bool lb_save_message(void) {
    return false;
}

void lb_check_connection(void)
{
    /* char* msg = bt_readline();
    if (lb_is_connect(msg)) {
        lb_display_connection();
        lb_read_message();
        lb_save_message();
        lb_set_new_as_current();
        lb_display_notice();
    } */
}
        
void lb_check_user_request(void)
{
    int key = get_key();
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

    if (key) {
        lb_display_message();
    }

    // wait until button is released
    while (get_key() != 0) {
      //  _delay_ms(10);
    }

    led_off();
}

void lb_read_message(void) {

}

void lb_set_new_as_current(void) {

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
    char* buf = storage_get_buffer();
    lb_init();

    strcpy(buf, "Nachricht 1: Alpha42");
    storage_save_message();
 
    strcpy(buf, "Nachricht 2: Nemo15");
    storage_save_message();
 
    strcpy(buf, "Nachricht 3: Knuth33");
    storage_save_message();
    
   
    //lb_display_message();
    lb_serve();
    return 0;
}

