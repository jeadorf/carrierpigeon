#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "main.h"
#include "global.h"
#include "uart.h"

int main(void)
{
    int c;
    char* text;
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, 11059200UL));
    sei();

    lcd_init();
    lcd_clear();
    lcd_set_page(7);
    lcd_set_column(5);

    while (true)
    {
        //TODO: implement 'CONNECT' detection to only echo back relevant
        // stuff
        text = uart_readline();
        if (text != NULL)
        {
            lcd_display_string(text);
        }
    }
}

char* uart_readline(void)
{
    int c;
    static char* result = NULL;
    static size_t size = 1;
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
                size = 1;
                // return what we got now
                return result;
            }
        }
        else {
            // not a \r or \n, add it to our string
            result = realloc(result, ++size);
            result[size-2] = c;
            result[size-1] = '\0';
        }
    }
    // if we aren't done, always return NULL
    return NULL;
}
