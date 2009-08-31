#include "storage.h"
#include "stdio.h"
#include "assert.h"
#include "stdbool.h"
#include "string.h"

// TODO: move to separate lib
bool assert_true(char *msg, bool b)
{
    if (b == false) {
        printf("Failed: '%s'\n", msg);
        return false;
    }
    return true;
}

bool assert_false(char *msg, bool b)
{
    return assert_true(msg, !b);
}

bool assert_equals(char *msg, char expected, char actual)
{
    if (expected != actual) {
        printf("Failed: '%s'. Expected '%c', got '%c'.\n",
                    msg, expected, actual);
        return false;
    }
    return true;
}

bool assert_equals_int(char *msg, int expected, int actual)
{
    if (expected != actual) {
        printf("Failed: '%s'. Expected '%d', got '%d'.\n",
                    msg, expected, actual);
        return false;
    }

    return true;
}

bool assert_equals_string(char *msg, char *expected, char *actual)
{
    // TODO: use strcmp here
    int i;
    for (i = 0; *(expected+i) != '\0' && *(actual+i) != '\0'; i++) {
        if (*expected != *actual) {
            printf("Failed: '%s'. Expected '%s', got '%s'.\n",
                        msg, expected, actual);
            return false;
        }
    }
    if (*(expected+i) != *(actual+i)) {
        printf("Failed: '%s'. Expected '%s', got '%s'.\n",
                    msg, expected, actual);
        return false;
    }

    return true;
}

void setup_test_storage(char* test_name)
{
    printf("%s\n", test_name);
    mock_eeprom_reset();
    storage_reset();
}

void teardown_test_storage(char* test_name)
{
    // mock_eeprom_print();
}

void test_mock_eeprom_reset(void)
{
    printf("test_mock_eeprom_reset\n");
    eeprom_write(0, 'b');
    mock_eeprom_reset();
    assert_true("test_mock_eeprom_reset", eeprom_read(0) == 0);
}

void test_mock_eeprom_read_write(void)
{
    printf("test_mock_eeprom_read_write\n");
    unsigned char expected = 'a';
    eeprom_write(0, expected);
    assert_true("test_mock_eeprom_read_write", eeprom_read(0) == expected);
}

void test_storage_reset(void)
{
    setup_test_storage("test_storage_reset");
    storage_save_message();
    storage_reset();
    assert_equals_int("count should be zero", 0, storage_message_count());
    teardown_test_storage("test_storage_reset");
}

void test_message_save_read_text(void)
{
    setup_test_storage("test_message_save_read_text");

    char *expected_text = "FooBar";
    strcpy(storage_get_buffer(), expected_text);
    
    // flush buffer
    storage_save_message();
    // scramble buffer
    strcpy(storage_get_buffer(), "someotherstuff");
    // restore buffer
    storage_load_message(0);

    assert_equals_string("saved text should be restored correctly",
        expected_text, storage_get_buffer()); 

    teardown_test_storage("test_message_save_read");
}

void test_message_initial_state(void)
{
    setup_test_storage("test_message_initial_state");
    
    storage_save_message();
    assert_equals_int("state should be STATE_NEW", STATE_NEW, 
       storage_get_state(0));

    teardown_test_storage("test_message_initial_state");
}

void test_save_message_count(void)
{
    setup_test_storage("test_save_message_count");

    storage_save_message();
    assert_equals_int("count should be one", 1, storage_message_count());

    teardown_test_storage("test_message_initial_state");
}

void test_delete_message_count(void)
{
    setup_test_storage("test_delete_message_count");

    storage_save_message();
    storage_delete_message(0);
    assert_equals_int("count should be zero", 0, storage_message_count());

    teardown_test_storage("test_delete_message_count");
}

void test_save_two_messages_count(void)
{
    setup_test_storage("test_save_two_messages_count");

    storage_save_message();
    storage_save_message();
    assert_equals_int("count should be two", 2, storage_message_count());

    teardown_test_storage("test_save_two_messages_count");
}

void test_save_max_full_message(void)
{
    setup_test_storage("test_save_max_full_message");

    char *expected =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "12345678901234567890123456"
        "qwertzuiopasdfghjklyxcvbnm"
        "QWERTZUI";
    strcpy(storage_get_buffer(), expected);
    storage_save_message();
    storage_load_message(0);
    assert_equals_string("full message should be recovered",
                                expected, storage_get_buffer());

    teardown_test_storage("test_save_max_full_message");
}

/* will lead to segmentation fault
   no longer responsibility of storage
   management.
void test_save_too_long_message(void)
{
    printf("test_save_too_long_message\n");
    mock_eeprom_reset();
    storage_reset();

    char *expected =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "12345678901234567890123456"
        "qwertzuiopasdfghjklyxcvbnm"
        "QWERTZUI";
    char *text = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "12345678901234567890123456"
        "qwertzuiopasdfghjklyxcvbnm"
        "QWERTZUIopasd";
    storage_save_message(STATE_NEW, text);
    assert_equals_string("text should be truncated",
                expected, storage_load_message(0));
    mock_eeprom_print();
}*/

void test_save_too_many_messages(void)
{
    setup_test_storage("test_save_too_many_messages");

    strcpy(storage_get_buffer(), "FooBar");
    storage_save_message();
    strcpy(storage_get_buffer(), "Grazie");
    storage_save_message();
    strcpy(storage_get_buffer(), "DonaldK");
    storage_save_message();
    strcpy(storage_get_buffer(), "EdgarD");
    storage_save_message();
    strcpy(storage_get_buffer(), "WayOff1");
    bool wayoff1 = storage_save_message();
    strcpy(storage_get_buffer(), "WayOff2");
    bool wayoff2 = storage_save_message();

    assert_false("should indicate failure (1)", wayoff1);
    assert_false("should indicate failure (2)", wayoff2);

    storage_load_message(0);
    assert_equals_string("test1", "FooBar", storage_get_buffer());
    storage_load_message(1);
    assert_equals_string("test2", "Grazie", storage_get_buffer());
    storage_load_message(2);
    assert_equals_string("test3", "DonaldK", storage_get_buffer());
    storage_load_message(3);
    assert_equals_string("test4", "EdgarD", storage_get_buffer());

    teardown_test_storage("test_save_too_many_messages");
}

/* run the tests */
int main(void)
{
    test_mock_eeprom_reset();
    test_mock_eeprom_read_write();
    test_storage_reset();
    test_message_save_read_text();
    test_message_initial_state();
    test_save_message_count();
    test_delete_message_count();
    test_save_two_messages_count();
    test_save_max_full_message();
    // test_save_too_long_message();
    test_save_too_many_messages();
    return 0;
}

