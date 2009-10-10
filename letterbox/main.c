/** Main routine of the letterbox server. The server works like
 * a finite, deterministic automaton.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "global.h"
#include "uart.h"
#include "lcd.h"
#include "led.h"
#include "text.h"
#include "bt.h"
#include "storage.h"
#include "assert.h"

/** Enumerates all possible GUI pages */
enum uipage {
    /* The letterbox has established a connection to a client
       and is downloading a message. The user may not interact
       with the letterbox during communication. */
    connection,
    /* The letterbox has received messages that have not yet
       been read by the user. */
    notice,
    /* The letterbox shows a specific message and its state */
    message
};

/* Stores which UI page is displayed on the LCD at the moment */
static enum uipage current_uipage = message;

/**
 * Initializes all components required by the letterbox.
 */
void lb_init(void)
{
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, 11059200UL));
    sei();

    lcd_init();
    lcd_clear();
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
    
    current_uipage = connection;
}

void lb_display_notice(void) {
    assert_true("uipage should be connection", current_uipage == connection);
    current_uipage = notice;
}

void lb_display_message(void) {
    char* text = storage_get_buffer();  
    current_uipage = message;
}

bool lb_read_request_line(void) {

}

void lb_send_confirm(void) {

}

bool lb_save_message(void) {

}

void lb_send_server_ready(void) {

}

void lb_send_error(void) {

}

void lb_force_disconnect(void) {

}

void lb_check_connection(void)
{
    char* msg = bt_readline();
    if (lb_is_connect(msg)) {
        lb_display_connection();
        if (lb_read_request_line()) {
            lb_send_server_ready();
            lb_read_message();
            if (lb_save_message()) {
                lb_send_confirm();
                lb_set_new_as_current();
                lb_display_notice();
            } else {
                lb_send_error();
            }
        }
        lb_force_disconnect();
    }
}
        
void lb_check_user_request(void)
{
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

