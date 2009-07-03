/* 
 * Support for assertions. The assertions communicate failures by 
 * letting LEDs blink, and printing error messages to the last
 * 256 bytes in the EEPROM.
 *
 */

#include <stdbool.h>

void assert_equals(char* error_msg, bool expected, bool actual);

void assert_true(char* error_msg, bool actual);

void assert_false(char* error_msg, bool actual);

void fail(char* error_msg);
