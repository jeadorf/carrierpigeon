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
 * You do not write and read messages directly, instead, there is a
 * global buffer that is large enough to hold the text of one message.
 * There are methods to fill the buffer with a certain message read
 * from memory and to flush the buffer to the memory.
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
 * Saves a message on the EEPROM. The message text is expected to be
 * in the buffer. Returns false if memory is full, true on success.
 */
bool storage_save_message(void);

/* 
 * Retrieves the state of the message. Returns STATE_EMPTY if the
 * number of stored messages is less than the specified
 * message_number.
 */
unsigned char storage_get_state(unsigned int message_number);

/*
 * Reads the message into the buffer. Returns false, if the number of
 * stored messages is less than the specified message_number. Returns
 * true on success.
 */
bool storage_load_message(unsigned int message_number);

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

