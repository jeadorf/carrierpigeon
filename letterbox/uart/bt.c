#include <avr/io.h>
#include <stdlib.h>
#include "global.h"
#include "bt.h"
#include "uart.h"
#include "message.h"

static unsigned char position = 0;
static unsigned char cr = 0, lf = 0;

// Implementation
bool bt_readline(char *buf, int max_size, bool use_buffer);
// Writes a null byte and resets static variables
void bt_finish_and_reset(char *buf, bool use_buffer);

bool bt_readline_buffer(char* buf, int max_size)
{
    return bt_readline(buf, max_size, true);
}

bool bt_readline_message(int max_size) 
{

    return bt_readline(0, max_size, false);
}

bool bt_readline(char *buf, int max_size, bool use_buffer)
{
    // TODO: stop when max_size is reached and return unfinished line
    int c;

    if (max_size == 0) {
        return true;
    }

    c = uart_getc();
    if (!(c & UART_NO_DATA))
    {
        // Check whether we have read max_size chars
        if (position == max_size - 1) {
            bt_finish_and_reset(buf, use_buffer); 
        }

        // mask out all higher level bits that might be set
        c = c & 0xFF;

        if (c == '\r') {
            // found a \r, add it to the count
            cr++;
        }
        else if (c == '\n') {
            lf++;
            if (lf == 2 && cr == 2) {
                bt_finish_and_reset(buf, use_buffer);
                // return what we got now
                return true; 
            }
        }
        else {
            if (use_buffer) {
                // not a \r or \n, add it to our string
                buf[position] = c;
            } else {
                message_write_char(c); 
            }
            position++;
        }
    }
    // if we aren't done, always return NULL
    return false;
}

void bt_finish_and_reset(char* buf, bool use_buffer)
{
    if (use_buffer) {
        buf[position] = '\0';
    } else {
        message_write('\0');
    }
    
    cr = 0;
    lf = 0;
    position = 0;
}

