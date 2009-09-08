#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "main.h"
#include "global.h"
#include "uart.h"

int main(void)
{
    int c;
    char* text;
    bool connected = false;
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

            if (connected)
            {
                if (strncmp(text, "DISCONNECT  ", 12) == 0)
                {
                    connected = false;
                    lcd_display_string("noes");
                }
            }
            else
            {
                if (strncmp(text, "CONNECT  ", 9) == 0)
                {
                    connected = true;
                    lcd_display_string("yay");
                }
            }
        }
    }
}

char* uart_readline(void)
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
