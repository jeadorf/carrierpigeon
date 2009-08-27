#include "eeprom.h"
#include "assert.h"

void test_read_write_char(void);
void test_read_write_string(void);

int main(void)
{
    /* run test suite */
    test_read_write_char();
    test_read_write_string();
    return 0;
}

void test_read_write_char(void)
{
    char expected = 'j';
    char actual;
    eeprom_write(15, expected);
    actual = eeprom_read(15);
    assert_true("EEPROM read/write char operation failed.", actual == expected);
}

void test_read_write_string(void)
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
