#include "assert.h"
#include "eeprom.h"
#include "led.h"
// TODO: test assertions
// TODO: make assertion lib compile

void assert_true(char* error_msg, bool actual)
{
    if (actual == false)
    {
        fail(error_msg);
    }
}

void assert_false(char* error_msg, bool actual)
{
    if (actual)
    {
        fail(error_msg);
    }
}

void fail(char* error_msg)
{
    // Write message to EEPROM, truncate message if running
    // out of memory
    int i = 0;
    while (error_msg[i] != '\0' && (i < EEPROM_SIZE / 2))
    {
        eeprom_write(i + EEPROM_SIZE / 2, error_msg[i]);
        i++;
    }

    // Signal that something is wrong. Run forever.
    while (true)
    {
        blink();
    }
}
