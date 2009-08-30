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
 * Each time a message is deleted, the message numbers of all newer
 * message are decremented. Thus, message numbers do not qualify as
 * time-independent identifiers.
 */

#include <stdbool.h>

// see MESSAGE status byte
#define STATE_EMPTY 'e'
#define STATE_NEW   'n'
#define STATE_READ  'r'

/*
 * Saves a message on the EEPROM. Returns false if memory is full,
 * true on success.
 */
bool storage_save_message(unsigned char state, unsigned char *text);

/* 
 * Retrieves the state of the message. Returns STATE_EMPTY if the
 * number of stored messages is less than the specified
 * message_number.
 */
unsigned char storage_get_state(unsigned int message_number);

/*
 * Retrieves the message body. Returns a NULL pointer if the number
 * of stored messages is less than the specified message_number.
 */
unsigned char* storage_get_text(unsigned int message_number);

/*
 * Removes the n-th oldest message. Returns true, if deleted successfully
 * (i.e. there was at such a message)
 */
bool storage_delete_message(unsigned int message_number);

/*
 * Get the number of stored messages.
 */
unsigned int storage_message_count();

