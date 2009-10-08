#include "assert.h"
#include "text.h"
#include "led.h"
// TODO: test assertions
// TODO: make assertion lib compile

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

    // Signal that something is wrong. Run forever.
    while (true)
    {
        blink();
    }
}

