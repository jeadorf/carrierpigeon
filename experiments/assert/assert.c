#include "assert.h"

void assert_equals(char* error_msg, bool expected, bool actual) 
{
    if (expected != actual)
    {
        fail(error_msg);
    }
}

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
    // stop and communicate failure
    while (true)
    {
        blink();
    }
}
