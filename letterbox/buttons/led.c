#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "led.h"
#include "global.h"

void led_blink_times(uint8_t led, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        led_blink(led);
    }
}

void led_init(void)
{
    // initialize green led
    set_bit(DDRC, DDC4);
    // initialize TODO led
    set_bit(DDRC, DDC5);
    // initialize TODO led
    set_bit(DDRC, DDC6);

    // initialize
    set_bit(DDRB, DDB2);
    set_bit(DDRB, DDB3);
}

void led_on(uint8_t led)
{
    set_bit(PORTC, led);
}

void led_off(uint8_t led)
{
    clear_bit(PORTC, led);
}

void led_blink(uint8_t led)
{
    led_on(led);
    _delay_ms(WAIT_TIME);
    led_off(led);
    _delay_ms(WAIT_TIME);
}
