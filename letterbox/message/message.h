/*
 * Stores, retrieves and deletes messages to/from the memory.
 *
 * The messages are ordered by time of arrival, such that newer
 * messages can be distinguished from older ones. 
 *
 * The message status and text can be retrieved using a message
 * number that temporarily identifies the message. Valid message
 * numbers are within the range [0, storage_message_count()[
 *
 * This library uses a streaming pattern. A storage operation will
 * include an initial call to message_new, multiple calls to
 * message_write and a final call to message_close. A load operation
 * will include an initial call to message_open, multiple calls to
 * message_read and a final call to message_close. The streaming
 * pattern helps to reduce the necessary buffer size and thus reduces
 * SRAM consumption.
 * 
 * Each time a message is deleted, the message numbers of all newer
 * message are decremented. Thus, message numbers do not qualify as
 * time-independent identifiers.
 */

#include <stdbool.h>
#include <stdint.h>

// see MESSAGE status byte
#define STATE_EMPTY 'e'
#define STATE_NEW   'n'
#define STATE_READ  'r'

#define MAX_MESSAGES 4
#define MESSAGE_SIZE 120
#define MESSAGE_TEXT_LENGTH 112
#define MESSAGE_RESERVED_SIZE 7

/* 
* Creates a new message entry that is to be stored in the EEPROM. After
* creating a new message you can write text via message_write. Returns
* false if and only if storage is full.
*/
bool message_new(void);

/*
 * Writes a sequence of characters to the message entry that has been 
 * created with message_new. You may call this function more than once.
 * Beware: after MESSAGE_TEXT_LENGTH bytes have been written, subsequent
 * calls to this function will have no effect. Returns the number of
 * bytes that have been written to the message entry. Does not write
 * a '\0' byte!
 */
uint8_t message_write(char *text);

/*
 * Just like message_write, but writes only a single character.
 */
uint8_t message_write_char(char c);

/*
 * Opens a message entry that is stored in the EEPROM. Returns true, if
 * there is an entry with such a msg_num, false otherwise.
 */
bool message_open(uint8_t msg_num);

/*
 * Reads a byte from the message entry that has been opened with
 * message_open. The '\0' byte always marks the end of the message
 * content. Thus, you can safely read until the '\0' byte occurs.
 */
char message_read(void);

/*
 * (1) Storage operation: Closes the current message entry that has
 * been created with message_new and filled with text using message_write.
 * If message_write wrote less than MESSAGE_TEXT_LENGTH bytes than this
 * method will append the '\0' byte.
 *
 * (2) Load operation: Closes the current message entry that has been
 * opened using message_open. 
 */
void message_close();

/* 
 * Retrieves the state of the message. Returns STATE_EMPTY if the
 * number of stored messages is less than the specified
 * message_number.
 */
char message_state(uint8_t msg_num);

/*
 * Removes the n-th oldest message. Returns true, if deleted successfully
 * (i.e. there was at such a message)
 */
bool message_delete(uint8_t msg_num);

/*
 * Get the number of stored messages.
 */
uint8_t message_count(void);

/*
 * Returns true iff there is no message.
 */
bool message_empty(void);

/*
 * Returns true iff there is no space for another message.
 */
bool message_full(void);

/*
 * Writes all metadata to EEPROM. If you switch off the letterbox
 * after a call to this function you can restore state with
 * message_restore after switching the power on again. Messages
 * are persistent anyway but the pointers in SRAM must be
 * made persistent too.
 */
void message_serialize(void);

/*
 * Restores all metadata from EEPROM. See message_serizalize.
 */
void message_restore(void);

/*
 * Ensures that all metadata is valid. This check becomes necessary
 * when data is restored from the EEPROM. The EEPROM is a I/O source
 * and therefore possibly unreliable and it is not safe to make 
 * assumptions on what data is to expect at whatsoever address.
 * Random EEPROM content could break the application and if it restores
 * state when power is switched on, it might well become necessary to
 * flash a valid image to the EEPROM to make the letterbox usable again.
 */
void message_validate(void);

