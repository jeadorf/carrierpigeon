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
            lcd_clear();
            lcd_set_page(7);
            lcd_set_column(5);
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
        c = c & 0xFF;
        if (c == '\r') {
            cr++;
        }
        else if (c == '\n') {
            lf++;
            if (lf == 2)
            {
                cr = 0;
                lf = 0;
                size = 1;
                return result;
            }
        }
        else {
            result = realloc(result, ++size);
            result[size-2] = c;
            result[size-1] = '\0';
        }
    }
    return NULL;
}
