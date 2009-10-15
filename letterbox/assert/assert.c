#include "assert.h"
#include "text.h"
#include "led.h"
#include <util/delay.h>

void assert_true(char *error_msg, bool actual)
{
    if (actual == false)
    {
        fail(error_msg);
    }
}

void assert_false(char *error_msg, bool actual)
{
    if (actual)
    {
        fail(error_msg);
    }
}

void fail(char *error_msg)
{
    lcd_display_string("ERROR ");
    lcd_display_string(error_msg);

    while (true) {
        led_blink(LED_RED);
        led_blink(LED_RED);
        led_blink(LED_RED);
        _delay_ms(1000);
    }

}

