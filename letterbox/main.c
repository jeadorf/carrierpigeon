#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <avr/interrupt.h>
#include "global.h"
#include "uart.h"
#include "lcd.h"
#include "text.h"
#include "bt.h"

int main(void)
{
    char* text;
    bool connected = false;
    bool wait4connect = true;
    
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, 11059200UL));
    sei();

    lcd_init();
    lcd_clear();

    while (true)
    {
        //TODO: implement 'CONNECT' detection to only echo back relevant
        // stuff
        text = bt_readline();
        if (text != NULL)
        {
            lcd_display_string(text);

            if (connected)
            {
                if (strncmp(text, "DISCONNECT  ", 12) == 0)
                {
                    connected = false;
                    lcd_display_string("noes ");
                    wait4connect = true;
                } else {
                    lcd_display_string(text);
                }
            }
            else
            {
                if (strncmp(text, "CONNECT  ", 9) == 0)
                {
                    connected = true;
                    lcd_display_string("yay ");
                }
            }
        }
        else if (wait4connect) 
        {
            // does not work ....
            /*
            lcd_init();
            lcd_clear();
            lcd_set_page(7);
            lcd_set_column(5);
            */
            lcd_display_string("waiting for connection...");
            wait4connect = false; 
            
        }
            
    }
    
}
