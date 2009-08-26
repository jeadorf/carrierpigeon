#include "eeprom.h"
#include "assert.h"

int main()
{
    /* run test suite */
    test_read_write_char();
    test_read_write_string();
}

void test_read_write_char()
{
    char expected = "j";
    char actual;
    eeprom_write(15, expected);
    actual = eeprom_read(15);
    assert_true("EEPROM read/write operation failed.", actual == expected);
}

void test_read_write_string()
{
    char* expected = "hitchhikers\0";
    int i;
    char actual;
    for (i = 0; expected[i] != '\0'; i++)
    {
        eeprom_write(15 + i, expected[i]);
    }
    for (i = 0; expected[i] != '\0'; i++)
    {
        actual = eeprom_read(44 + i);
        assert_true("EEPROM read/write string operation failed.",
                expected[i] == actual);
    }
}
