/* 
 * Support for assertions. If an assertion fails, LEDs will blink in
 * order to signal the programmer that something has gone wrong. The
 * error message is stored in the second half of the EEPROM, and 
 * then halts by entering an infinite loop. This has the advantage of
 * being easy and makes this assertion tool only rely on LED and EEPROM
 * code. Obvious disadvantage: the assertion mechanism may overwrite
 * data in the EEPROM.
 */

#include <stdbool.h>

#define EEPROM_SIZE 512

void assert_true(char* error_msg, bool actual);

void assert_false(char* error_msg, bool actual);

void fail(char* error_msg);
