#include <stdbool.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "main.h"
#include "global.h"
#include "uart.h"

int main(void)
{
    int c;
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, 11059200UL));
    sei();

    lcd_init();
    lcd_clear();
    lcd_set_page(0);
    lcd_set_column(10);
    lcd_draw_char('S');
    // set the prescaler of the timer to 1024
    set_bit(TCCR1B, CS12);
    set_bit(TCCR1B, CS10);

    while (true)
    {
        c = uart_getc();
        reset_lcd();
        if (!(c & UART_NO_DATA))
        {
            lcd_draw_char(c & 0xFF);
        }
    }
}

void reset_lcd(void)
{
    // fire every 2 seconds
    if (TCNT1 >= 21600) {
        lcd_clear();
        lcd_set_page(0);
        lcd_set_column(10);
        lcd_draw_char('S');
        // reset timer to prevent overflow
        TCNT1 = 0;
    }
}
