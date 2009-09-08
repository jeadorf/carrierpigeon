#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "global.h"
#include "uart.h"

int main(void)
{
    int c;
    char* text;
    bool connected = false;
    /*
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
    */
}
