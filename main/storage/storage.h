/*
 * Stores, retrieves and deletes messages to/from the EEPROM memory.
 * A message takes 120 bytes and consists of the following parts:
 * 
 *              MESSAGE
 *
 *   Byte   Name            Purpose
 * -------------------------------------------------------------------
 *      0   status          0: Memory block is empty.
 *                          1: Message is new.
 *                          2: Message has been read.
 *    1-7   <reserved>      Reserved for further amendments.
 *  8-119   body            The actual message text.
 *
 *
 * The messages are ordered by time of arrival, such that newer
 * messages can be distinguished from older ones. This requires
 * basic storage management.
 *
 *
 *              MEMORY
 *
 *    Byte  Name            Purpose
 * -------------------------------------------------------------------
 *    0-31  nodes           Stores pointers to messages. Byte 0 stores
 *                          the pointer to the oldest message. A
 *                          pointer with value zero is defined as the
 *                          null pointer, a pointer with value one is
 *                          defined as the pointer to the first block
 *                          in the message memory partition.
 *  32-151  message         Contains a message (see MESSAGE). The
 * 152-271  message         position of the messages does not carry
 * 272-391  message         any meaning, a free slot is a free slot.
 * 392-511  message
 *
 * The nodes section represents a stack. Each byte contains a memory
 * address that points to the start offset of a message in memory.
 * Each time a message is deleted, the nodes section will be
 * reorganized. Message numbers change and thus do not qualify as
 * identifiers, yet older messages always carry smaller message
 * numbers than newer ones.
 */

#include <stdbool.h>

// see MESSAGE status byte
#define STATE_EMPTY 0
#define STATE_NEW   1
#define STATE_READ  2

// This is where the first message memory block begins
#define MESSAGE_STORAGE_OFFSET 7

/*
 * Saves a message on the EEPROM. Returns false if memory is full,
 * true on success.
 */
bool storage_save_message(byte state, *byte text);

/* 
 * Retrieves the state of the message. Returns STATE_EMPTY if the
 * number of stored messages is less than the specified
 * message_number.
 */
byte storage_get_state(byte message_number);

/*
 * Retrieves the message body. Returns a NULL pointer if the number
 * of stored messages is less than the specified message_number.
 */
*byte storage_get_text(byte message_number);

bool storage_is_empty();

bool storage_is_full();

