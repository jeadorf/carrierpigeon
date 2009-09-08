#include <stdlib.h>
#include "bt.h"
#include "uart.h"

char* bt_readline(void)
{
    int c;
    // TODO: MAX_MESSAGE_LENGTH
    static char result[112+1];
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
                return result;
            }
        }
        else {
            // not a \r or \n, add it to our string
            result[position++] = c;
            result[position] = '\0';
        }
    }
    // if we aren't done, always return NULL
    return NULL;
}
