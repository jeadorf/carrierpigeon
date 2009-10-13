#include "storage.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "assert.h"

// TODO: move to separate lib
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

/* helper */
void helper_storage_read_buf(char buffer[])
{
    int i = 0;
    do {
        buffer[i] = storage_read();
        i++;
    } while (buffer[i-1] != '\0'); 
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
    // save a message
    bool ok = storage_new();
    assert_true("could not create new message", ok);
    storage_write("FooBar");
    storage_close();
    // reset and clear memory
    storage_reset();
    assert_equals_int("count should be zero", 0, storage_message_count());
    teardown_test_storage("test_storage_reset");
}

void test_message_save_read_text(void)
{
    setup_test_storage("test_message_save_read_text");

    // write message
    char *expected_text = "FooBar";
    char restored_text[6];
    char c;
    int i = 0;
    storage_new();
    storage_write(expected_text);
    storage_close();
    // restore buffer
    storage_open(0);
    helper_storage_read_buf(restored_text);
    storage_close();

    assert_equals_string("saved text should be restored correctly",
        expected_text, restored_text); 

    teardown_test_storage("test_message_save_read");
}

void test_message_initial_state(void)
{
    setup_test_storage("test_message_initial_state");
    
    storage_new();
    storage_close();
    assert_equals_int("state should be STATE_NEW", STATE_NEW, 
       storage_get_state(0));

    teardown_test_storage("test_message_initial_state");
}

void test_save_message_count(void)
{
    setup_test_storage("test_save_message_count");

    storage_new();
    storage_close();
    assert_equals_int("count should be one", 1, storage_message_count());

    teardown_test_storage("test_message_initial_state");
}

void test_delete_message_count(void)
{
    setup_test_storage("test_delete_message_count");

    storage_new();
    storage_close();
    storage_delete_message(0);
    assert_equals_int("count should be zero", 0, storage_message_count());

    teardown_test_storage("test_delete_message_count");
}

void test_save_two_messages_count(void)
{
    setup_test_storage("test_save_two_messages_count");

    storage_new();
    storage_close();
    storage_new();
    storage_close();
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
    char restored_text[112];

    storage_new();
    storage_write(expected);
    storage_close();

    storage_open(0);
    helper_storage_read_buf(restored_text);
    storage_close();

    assert_equals_string("full message should be recovered",
                                expected, restored_text); 

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

    storage_new();
    storage_write("FooBar");
    storage_close();

    storage_new();
    storage_write("Grazie");
    storage_close();

    storage_new();
    storage_write("DonaldK");
    storage_close();

    storage_new();
    storage_write("EdgarD");
    storage_close();

    bool wayoff1 = storage_new();
    storage_write("WayOff1");
    storage_close();

    bool wayoff2 = storage_new();
    storage_write("WayOff2");
    storage_close();

    assert_false("should indicate failure (1)", wayoff1);
    assert_false("should indicate failure (2)", wayoff2);

    char restored_text[32];

    storage_open(0);
    helper_storage_read_buf(restored_text);
    storage_close();
    assert_equals_string("test1", "FooBar", restored_text); 

    storage_open(1);
    helper_storage_read_buf(restored_text);
    storage_close(); 
    assert_equals_string("test2", "Grazie", restored_text); 

    storage_open(2);
    helper_storage_read_buf(restored_text);
    storage_close(); 
    assert_equals_string("test3", "DonaldK", restored_text); 

    storage_open(3);
    helper_storage_read_buf(restored_text);
    storage_close(); 
    assert_equals_string("test4", "EdgarD", restored_text); 

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

