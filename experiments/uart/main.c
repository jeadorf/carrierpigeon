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
    // set the prescaler of the timer to 1024
    set_bit(TCCR1B, CS12);
    set_bit(TCCR1B, CS10);


    while (true)
    {
        //TODO: implement 'CONNECT' detection to only echo back relevant
        // stuff
        text = uart_readline();
        if (text != NULL)
        {
            reset_lcd();
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

void reset_lcd(void)
{
    // fire every 2 seconds
    if (TCNT1 >= 21600)
    {
        lcd_clear();
        lcd_set_page(7);
        lcd_set_column(5);
        // reset timer to prevent overflow
        TCNT1 = 0;
    }
}
