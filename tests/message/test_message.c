#include "message.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "assert.h"

// TODO: move to separate lib
void setup_test_message(char* test_name)
{
    printf("%s\n", test_name);
    mock_eeprom_reset();
    message_reset();
}

void teardown_test_message(char* test_name)
{
    // mock_eeprom_print();
}

/* helper */
void helper_message_read_buf(char buffer[])
{
    int i = 0;
    do {
        buffer[i] = message_read();
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

void test_message_reset(void)
{
    setup_test_message("test_message_reset");
    // save a message
    bool ok = message_new();
    assert_true("could not create new message", ok);
    message_write("FooBar");
    message_close();
    // reset and clear memory
    message_reset();
    assert_equals_int("count should be zero", 0, message_count());
    teardown_test_message("test_message_reset");
}

void test_message_save_read_text(void)
{
    setup_test_message("test_message_save_read_text");

    // write message
    char *expected_text = "FooBar";
    char restored_text[7];
    char c;
    int i = 0;

    message_new();
    message_write(expected_text);
    message_close();

    // restore buffer
    message_open(0);
    helper_message_read_buf(restored_text);
    message_close();

    assert_equals_string("saved text should be restored correctly",
        expected_text, restored_text); 

    teardown_test_message("test_message_save_read");
}

void test_message_initial_state(void)
{
    setup_test_message("test_message_initial_state");
    
    message_new();
    message_close();
    assert_equals_int("state should be STATE_NEW", STATE_NEW, 
       message_state(0));

    teardown_test_message("test_message_initial_state");
}

void test_save_message_count(void)
{
    setup_test_message("test_save_message_count");

    message_new();
    message_close();
    assert_equals_int("count should be one", 1, message_count());

    teardown_test_message("test_message_initial_state");
}

void test_delete_message_count(void)
{
    setup_test_message("test_delete_message_count");

    message_new();
    message_close();
    message_delete(0);
    assert_equals_int("count should be zero", 0, message_count());

    teardown_test_message("test_delete_message_count");
}

void test_save_two_messages_count(void)
{
    setup_test_message("test_save_two_messages_count");

    message_new();
    message_close();
    message_new();
    message_close();
    assert_equals_int("count should be two", 2, message_count());

    teardown_test_message("test_save_two_messages_count");
}

void test_save_max_full_message(void)
{
    setup_test_message("test_save_max_full_message");

    char *expected =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "12345678901234567890123456"
        "qwertzuiopasdfghjklyxcvbnm"
        "QWERTZUI";
    char restored_text[112];

    message_new();
    message_write(expected);
    message_close();

    message_open(0);
    helper_message_read_buf(restored_text);
    message_close();

    assert_equals_string("full message should be recovered",
                                expected, restored_text); 

    teardown_test_message("test_save_max_full_message");
}

/* will lead to segmentation fault
   no longer responsibility of message
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
    setup_test_message("test_save_too_many_messages");

    message_new();
    message_write("FooBar");
    message_close();

    message_new();
    message_write("Grazie");
    message_close();

    message_new();
    message_write("DonaldK");
    message_close();

    message_new();
    message_write("EdgarD");
    message_close();

    bool wayoff1 = message_new();
    message_write("WayOff1");
    message_close();

    bool wayoff2 = message_new();
    message_write("WayOff2");
    message_close();

    assert_false("should indicate failure (1)", wayoff1);
    assert_false("should indicate failure (2)", wayoff2);

    char restored_text[32];

    message_open(0);
    helper_message_read_buf(restored_text);
    message_close();
    assert_equals_string("test1", "FooBar", restored_text); 

    message_open(1);
    helper_message_read_buf(restored_text);
    message_close(); 
    assert_equals_string("test2", "Grazie", restored_text); 

    message_open(2);
    helper_message_read_buf(restored_text);
    message_close(); 
    assert_equals_string("test3", "DonaldK", restored_text); 

    message_open(3);
    helper_message_read_buf(restored_text);
    message_close(); 
    assert_equals_string("test4", "EdgarD", restored_text); 

    teardown_test_message("test_save_too_many_messages");
}

void test_read_message_until_null_byte_occurs(void)
{
    setup_test_message("test_read_message_until_null_byte_occurs");

    char expected_text[] = "alpha";

    message_new();
    message_write(expected_text);
    message_close();

    message_open(0);
    char restored_text[25];
    char c;
    int i = 0;
    while ((c = message_read()) != '\0') {
        restored_text[i] = c;
        i++;
    }
    restored_text[i] = '\0';
    assert_equals_string("problem", expected_text, restored_text);
    
    teardown_test_message("test_read_message_until_null_byte_occurs");
}

void test_return_null_byte_on_invalid_read(void)
{
    setup_test_message("test_return_null_byte_on_invalid_read");

    assert_true("expected \\0 byte", '\0' == message_read());

    teardown_test_message("test_return_null_byte_on_invalid_read");
}
void test_message_restore(void)
{
    setup_test_message("test_message_serialize");

    message_new();
    message_write("alpha"); 
    message_close();

    message_new();
    message_write("beta");
    message_close();

    message_serialize();
    
    message_new();
    message_write("gamma");
    message_close();

    message_restore();

    assert_true("storage_count not correctly serialized/restored!", 2 == message_count());

    char restored_text[16];
    message_open(0);
    helper_message_read_buf(restored_text);
    assert_equals_string("problem", "alpha", restored_text);

    message_open(1);
    helper_message_read_buf(restored_text);
    assert_equals_string("problem", "beta", restored_text);

    teardown_test_message("test_message_serialize");
}

void test_message_validate(void)
{
    setup_test_message("test_message_validate");
    
    // Fill EEPROM with corrupt data
    int i;
    for (i = 0; i < 512; i++) {
        eeprom_write(66);
    }

    message_restore();

    assert_true("message validation corrupt", message_count() == 0);

    // Create storage_nodes with duplicates
    eeprom_write(480, 4);
    eeprom_write(481, 1);
    eeprom_write(482, 3);
    eeprom_write(483, 4);
    eeprom_write(484, 3);

    assert_true("duplicates not detected", message_count() == 0);

    // Check for invalid block values
    eeprom_write(480, 4);
    eeprom_write(481, 1);
    eeprom_write(482, 3);
    eeprom_write(483, MAX_MESSAGES + 1);
    eeprom_write(484, 3);

    assert_true("invalid block values not detected", message_count() == 0);

    teardown_test_message("test_message_validate");
}

/* run the tests */
int main(void)
{
    test_mock_eeprom_reset();
    test_mock_eeprom_read_write();
    test_message_reset();
    test_message_save_read_text();
    test_message_initial_state();
    test_save_message_count();
    test_delete_message_count();
    test_save_two_messages_count();
    test_save_max_full_message();
    // test_save_too_long_message();
    test_save_too_many_messages();
    test_read_message_until_null_byte_occurs();
    test_return_null_byte_on_invalid_read();
    test_message_restore();
    test_message_validate();
    return 0;
}

