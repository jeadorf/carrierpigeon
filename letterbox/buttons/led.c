#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "led.h"
#include "commons.h"

void led_blink_times(uint8_t led, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        led_blink(led);
    }
}

void led_init(void)
{
    // initialize green led
    set_bit(DDRC, DDC4);
    // initialize blue led
    set_bit(DDRC, DDC5);
    // initialize red led
    set_bit(DDRC, DDC6);

    // initialize
    set_bit(DDRB, DDB2);
    set_bit(DDRB, DDB3);
}

void led_on(uint8_t led)
{
    switch (led) {
        case LED_GREEN:
        case LED_BLUE:
        case LED_RED:
            set_bit(PORTC, led);
            break;
        case LED_LINE_RED:
        case LED_LINE_BLUE:
            set_bit(PORTB, led);
            break;
    }
}

void led_off(uint8_t led)
{
    switch (led) {
        case LED_GREEN:
        case LED_BLUE:
        case LED_RED:
            clear_bit(PORTC, led);
            break;
        case LED_LINE_RED:
        case LED_LINE_BLUE:
            clear_bit(PORTB, led);
            break;
    }
}

void led_blink(uint8_t led)
{
    led_on(led);
    _delay_ms(WAIT_TIME);
    led_off(led);
    _delay_ms(WAIT_TIME);
}
