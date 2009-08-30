#include "storage.h"
#include "stdio.h"
#include "assert.h"
#include "stdbool.h"

// TODO: move to separate lib
bool assert_true(char *msg, bool b)
{
    if (b == false) {
        printf("Failed: '%s'\n", msg);
        return false;
    }
    return true;
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
    printf("test_storage_reset\n");
    storage_save_message(STATE_NEW, "FooBar");
    storage_reset();
    assert_equals_int("count should be zero", 0, storage_message_count());
}

void test_message_save_read_text(void)
{
    printf("test_message_save_read_text\n");
    mock_eeprom_reset();
    storage_reset();
    char *expected_text = "FooBar";
    storage_save_message(STATE_NEW, expected_text); 
    char *actual_text = storage_get_text(0);
    assert_true("read text should not be null", actual_text != NULL);
    assert_equals_string("saved text should be restored correctly",
        expected_text, actual_text); 
    mock_eeprom_print();
}

void test_message_initial_state(void)
{
    printf("test_message_initial_state\n");
    mock_eeprom_reset();
    storage_reset();
    storage_save_message(STATE_NEW, "FooBar");
    assert_equals_int("state should be STATE_NEW", STATE_NEW, 
       storage_get_state(0));
    mock_eeprom_print();
}

void test_save_message_count(void)
{
    printf("test_save_message_count\n");
    mock_eeprom_reset();
    storage_reset();
    storage_save_message(STATE_NEW, "FooBar");
    assert_equals_int("count should be one", 1, storage_message_count());
    mock_eeprom_print();
}

void test_delete_message_count(void)
{
    printf("test_delete_message_count\n");
    mock_eeprom_reset();
    storage_reset();
    storage_save_message(STATE_NEW, "FooBar");
    storage_delete_message(0);
    assert_equals_int("count should be zero", 0, storage_message_count());
    mock_eeprom_print();
}

void test_save_two_messages_count(void)
{
    printf("test_save_two_messages_count\n");
    mock_eeprom_reset();
    storage_reset();
    storage_save_message(STATE_NEW, "FooBar");
    storage_save_message(STATE_NEW, "DonaldK");
    assert_equals_int("count should be two", 2, storage_message_count());
    mock_eeprom_print();
}

void test_save_max_full_message(void)
{
    printf("test_save_max_full_message\n");
    mock_eeprom_reset();
    storage_reset();
    char *expected =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "12345678901234567890123456"
        "qwertzuiopasdfghjklyxcvbnm"
        "QWERTZUI";
    storage_save_message(STATE_NEW, expected);
    char *actual = storage_get_text(0);
    assert_equals_string("full message should be recovered",
                                expected, actual);
    mock_eeprom_print();
}

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
                expected, storage_get_text(0));
    mock_eeprom_print();
}

void test_save_too_many_messages(void)
{
    printf("test_save_too_many_messages\n");
    mock_eeprom_reset();
    storage_reset();
    storage_save_message(STATE_NEW, "FooBar");
    storage_save_message(STATE_NEW, "Grazie");
    storage_save_message(STATE_NEW, "DonaldK");
    storage_save_message(STATE_NEW, "EdgarD");
    storage_save_message(STATE_NEW, "Bottom");
    storage_save_message(STATE_NEW, "hitchhiker");
    assert_equals_string("test1", "FooBar", storage_get_text(0));
    assert_equals_string("test2", "Grazie", storage_get_text(1));
    assert_equals_string("test3", "DonaldK", storage_get_text(2));
    assert_equals_string("test4", "EdgarD", storage_get_text(3));
    mock_eeprom_print();
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
    test_save_too_long_message();
    test_save_too_many_messages();
    return 0;
}

