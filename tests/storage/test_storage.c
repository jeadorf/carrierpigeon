#include "storage.h"
#include "stdio.h"
#include "assert.h"
#include "stdbool.h"

// TODO: move to separate lib
void assert_true(char *msg, bool b)
{
    if (b == false) {
        printf("Failed: '%s'\n", msg);
    }
}

/* testing the mock utility */
void test_mock_eeprom_reset(void)
{
    eeprom_write(0, 'b');
    mock_eeprom_reset();
    assert_true("test_mock_eeprom_reset", eeprom_read(0) == 0);
}

/* testing the mock utility */
void test_mock_eeprom_read_write(void)
{
    unsigned char expected = 'a';
    eeprom_write(0, expected);
    assert_true("test_mock_eeprom_read_write", eeprom_read(0) == expected);
}

/* test store message */
void test_storage_save_text(void)
{
    mock_eeprom_reset();
    char *text = "FooBar";
    storage_save_message(STATE_NEW, text); 
    char *rtext = storage_get_text(0);
    // TODO: fixme
}


/* run the tests */
int main(void)
{
    /*
    int i;
    eeprom_write(0, 'b');
    printf("%d\n", eeprom_read(0));
    storage_save_message(STATE_NEW, "Foo Bar");
    storage_save_message(STATE_NEW, "All your base"
           " are belong to us!");
    mock_eeprom_print(); */

    test_mock_eeprom_reset();
    test_mock_eeprom_read_write();
    return 0;
}

