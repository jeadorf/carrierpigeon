#include <stdbool.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "main.h"
#include "uart.h"

int main(void) {
    int c;
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,11059200UL));
    sei();

    lcd_init();
    lcd_clear();
    lcd_set_page(0);
    lcd_set_column(10);
    lcd_draw_char('S');

    while (true) {
        c = uart_getc();
        if (!(c & UART_NO_DATA)) {
            lcd_draw_char(c & 0xFF);
        }
    }
}
