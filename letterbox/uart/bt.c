#include <avr/io.h>
#include <stdlib.h>
#include "global.h"
#include "bt.h"
#include "uart.h"

bool bt_readline(char* buf, int max_size)
{
    int c;
    static unsigned char position = 0;
    static unsigned char cr = 0, lf = 0;

    c = uart_getc();
    if (!(c & UART_NO_DATA))
    {
        // mask out all higher level bits that might be set
        c = c & 0xFF;

        if (c == '\r') {
            // found a \r, add it to the count
            cr++;
        }
        else if (c == '\n') {
            lf++;
            if (lf == 2 && cr == 2)
            {
                // we found the terminal \n
                // reset all static variables back to their initial values
                cr = 0;
                lf = 0;
                position = 0;
                // return what we got now
                return buf; 
            }
        }
        else {
            // not a \r or \n, add it to our string
            buf[position++] = c;
            buf[position] = '\0';
        }
    }
    // if we aren't done, always return NULL
    return NULL;
}
