#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include "global.h"

void led_blink_times(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        led_blink();
    }
}

void led_on(void)
{
    set_bit(DDRC, DDC4);
    set_bit(PORTC, PC4);
}

void led_off(void)
{
    clear_bit(PORTC, PC4);
}


void led_blink(void)
{
    led_on();
    _delay_ms(WAIT_TIME);
    led_off();
    _delay_ms(WAIT_TIME);
}
