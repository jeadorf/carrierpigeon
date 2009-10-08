/** Main routine of the letterbox server. The server works like
 * finite, deterministic automaton.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "global.h"
#include "uart.h"
#include "lcd.h"
#include "led.h"
#include "text.h"
#include "bt.h"
#include "storage.h"

/**
 * Initializes all components required by the letterbox.
 */
void letterbox_init(void)
{
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, 11059200UL));
    sei();

    lcd_init();
    lcd_clear();
}

bool letterbox_is_connect(char* msg)
{
    return msg != NULL && strncmp(msg, "CONNECT  ", 9) == 0;
}

bool letterbox_is_disconnect(char* msg)
{
    return msg != NULL && strncmp(msg, "DISCONNECT  ", 12) == 0;
}

void letterbox_wait_for_connection(void)
{
    char* msg;
    while (true) {
        msg = bt_readline();
        if (letterbox_is_connect(msg)) {
            return;
        } else {
            // Yield and give the user a chance to trigger some
            // action.
            letterbox_handle_user_request();
        }
    }
}

void letterbox_retrieve_message(void)
{
    char* msg;
    int i, pos;
    char* storage = storage_get_buffer();
    while (pos < MESSAGE_TEXT_LENGTH) {
        msg = bt_readline();
        if (letterbox_is_disconnect(msg)) {
            storage[pos+1] = '\0';
            break;
        } else if (msg != NULL) {
            for (i = 0; msg[i] != '\0' && pos < MESSAGE_TEXT_LENGTH; i++) { 
                storage[pos++] = msg[i];
            }
        }
    }
    lcd_display_string(storage);
    led_on();
    // FIXME:
    while (true) {}
    // storage_save_message();
}

void letterbox_handle_client(void)
{
    char* msg;
    letterbox_wait_for_connection();
    letterbox_retrieve_message(); 
}

void letterbox_handle_user_request(void)
{
    // FIXME 
}

/**
 * Lifecycle of the letterbox.
 */
void letterbox_serve(void)
{
    while (true) {
        letterbox_handle_client();
        letterbox_handle_user_request();
    }
}

/** Letterbox life cycle. */
int main(void)
{
    letterbox_init();
    letterbox_serve();
    return 0;
}

