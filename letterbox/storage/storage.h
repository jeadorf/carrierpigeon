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
 * include an initial call to storage_new, multiple calls to
 * storage_write and a final call to storage_close. A load operation
 * will include an initial call to storage_open, multiple calls to
 * storage_read and a final call to storage_close. The streaming
 * pattern helps to reduce the necessary buffer size and thus reduces
 * SRAM consumption.
 * 
 * Each time a message is deleted, the message numbers of all newer
 * message are decremented. Thus, message numbers do not qualify as
 * time-independent identifiers.
 */

#include <stdbool.h>

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
* creating a new message you can write text via storage_write. Returns
* false if and only if storage is full.
*/
bool storage_new(void);

/*
 * Writes a sequence of characters to the message entry that has been 
 * created with storage_new. You may call this function more than once.
 * Beware: after MESSAGE_TEXT_LENGTH bytes have been written, subsequent
 * calls to this function will have no effect. Returns the number of
 * bytes that have been written to the message entry.
 */
int storage_write(char *buf);

/*
 * Opens a message entry that is stored in the EEPROM. Returns true, if
 * there is an entry with such a msg_num, false otherwise.
 */
bool storage_open(int msg_num);

/*
 * Reads a byte from the message entry that has been opened with
 * storage_open. The '\0' byte always marks the end of the message
 * content. Thus, you can safely read until the '\0' byte occurs.
 */
char storage_read(void);

/*
 * (1) Storage operation: Closes the current message entry that has
 * been created with storage_new and filled with text using storage_write.
 * If storage_write wrote less than MESSAGE_TEXT_LENGTH bytes than this
 * method will append the '\0' byte.
 *
 * (2) Load operation: Closes the current message entry that has been
 * opened using storage_open. 
 */
void storage_close();

/* 
 * Retrieves the state of the message. Returns STATE_EMPTY if the
 * number of stored messages is less than the specified
 * message_number.
 */
unsigned char storage_get_state(unsigned int message_number);

/*
 * Removes the n-th oldest message. Returns true, if deleted successfully
 * (i.e. there was at such a message)
 */
bool storage_delete_message(unsigned int message_number);

/*
 * Gets a pointer to the current buffer. The buffer is 112 bytes wide,
 * writing beyond 112 bytes will result in segmentation fault.
 */
char* storage_get_buffer(void);

/*
 * Get the number of stored messages.
 */
unsigned int storage_message_count(void);

/*
 * Returns true iff there is no message.
 */
bool storage_is_empty(void);

/*
 * Returns true iff there is no space for another message.
 */
bool storage_is_full(void);

