/*
 * A message takes 120 bytes and consists of the following parts:
 * 
 *              MESSAGE
 *
 *   Byte   Name            Purpose
 * -------------------------------------------------------------------
 *      0   status          0: Memory block is EMPTY.
 *                          1: Message is NEW.
 *                          2: Message has been READ.
 *    1-7   <reserved>      Reserved for further amendments.
 *  8-119   body            The actual message text. The message text
 *                          is either delimited by the end of the block
 *                          or by a \0 character.
 *
 *
 *              MEMORY
 *
 *    Byte  Name            Purpose
 * -------------------------------------------------------------------
 *   0-119  message         Contains a message (see MESSAGE). The
 * 120-239  message         position of the messages does not carry
 * 240-359  message         any meaning, a free slot is a free slot.
 * 360-479  message
 * 480-512  <reserved>      Reserved
 *
 * There is a list of pointers that maintains the order of the messages.
 * The first entry refers to the oldest message. Its value points to the
 * memory block where the message is stored. Deleting a message will
 * just delete the node in the list of pointers.
 */

#include "message.h"
#include "eeprom.h"
#include "stdint.h"

// Memory state: List of nodes that point to message blocks. Zero marks a free slot
// in the list. The first zero denotes the end of the list. A value
// n denotes that the message is actually stored in the n-th memory
// block (counting begins with 1)
uint8_t storage_nodes[MAX_MESSAGES] = { 0, 0, 0, 0 };

// Memory state: Points to the next free slot in storage_nodes 
uint8_t storage_count = 0;

// Streaming state: Points to the current read/write position
uint16_t pos;
uint16_t offset;
bool open = false;

void message_validate(void);

bool message_new(void)
{
    uint8_t i;
    uint8_t block;
    bool found_free;
    if (!message_full()) {
        // Find free memory block
        // Check all blocks, do not do this by reading the
        // EEPROM message status bit, it should be faster this way.
        for (block = 1; block <= MAX_MESSAGES; block++) {
            found_free = true;
            // Look whether one node points to this block.
            for (i = 0; i < MAX_MESSAGES; i++) {
                if (storage_nodes[i] == block) {
                    found_free = false;
                    break;
                }
            }
            if (found_free) {
                // Found a free memory block
                storage_nodes[storage_count] = block;
                storage_count++;
                offset = (block - 1) * MESSAGE_SIZE;

                // Write status byte and skip reserved
                // message part
                eeprom_write(offset, STATE_NEW);
                offset += MESSAGE_RESERVED_SIZE + 1;
                pos = 0;
                open = true;
                break;
            }
        }
        return true;
    }

    block = -1;
    return false;
}

uint8_t message_write_char(char c)
{
    if (open) {
        eeprom_write(offset + pos, c);
        pos++;
        return 1;
    } else {
        return 0;
    }
}

uint8_t message_write(char *buf)
{
    uint8_t i = 0;
    while (buf[i] != '\0') {
        message_write_char(buf[i]);
        i++;
    }
    return i;
}

bool message_open(uint8_t msg_num)
{
    uint8_t block;
    if (msg_num < message_count()) {
        block = storage_nodes[msg_num];
        offset = (block - 1) * MESSAGE_SIZE + MESSAGE_RESERVED_SIZE + 1;
        pos = 0;
        open = true;
        return true;
    } else {
        return false;
    }
}

char message_read(void)
{
    char c;
    if (!open || pos == MESSAGE_TEXT_LENGTH) {
        c = '\0';
    } else {
        c = eeprom_read(offset + pos);
        pos++;
    }
    return c;
}

void message_close(void)
{
    open = false;
}

bool message_empty(void)
{
    return storage_count == 0;
}

bool message_full(void)
{
    return storage_count == MAX_MESSAGES;
}

char message_state(uint8_t msg_num)
{
    if (msg_num < storage_count) {
        uint8_t block = storage_nodes[msg_num];
        uint16_t mem_addr = (block - 1) * MESSAGE_SIZE;
        return eeprom_read(mem_addr);
    }

    return STATE_EMPTY;
}

bool message_delete(uint8_t msg_num)
{
    if (msg_num < storage_count) {
        // Shift all message pointers left, thus removing one pointer.
        uint8_t i;
        for (i = msg_num + 1; i < storage_count; i++) {
            storage_nodes[i - 1] = storage_nodes[i];
        }
        storage_nodes[storage_count - 1] = 0;
        storage_count--;
        return true;
    } else {
        return false;
    }
}

uint8_t message_count(void)
{
    return storage_count;
}

void message_serialize(void)
{
    uint8_t i;
    eeprom_write(480, storage_count);
    for (i = 0; i < MAX_MESSAGES; i++) {
        eeprom_write(481 + i, storage_nodes[i]);
    }
}

void message_restore(void)
{
    uint8_t i;
    storage_count = eeprom_read(480);
    for (i = 0; i < MAX_MESSAGES; i++) {
        storage_nodes[i] = eeprom_read(481 + i);
        if (storage_nodes[i] > MAX_MESSAGES) {
            storage_nodes[i] = 0;
        }
    }

    // Ensure that the letterbox starts up with a valid state
    message_validate();
}

void message_validate(void)
{
    uint8_t i, j;
    // Check storage count
    if (storage_count > MAX_MESSAGES) {
        storage_count = 0;
        return;
    }
    // Check for obviously invalid values
    for (i = 0; i < MAX_MESSAGES; i++) {
        if (storage_nodes[i] > MAX_MESSAGES) {
            storage_count = 0;
            return;
        }
    }
    // Check for duplicates
    for (i = 0; i < storage_count; i++) {
        for (j = 0; j < storage_count; j++) {
            if (i != j && storage_nodes[i] == storage_nodes[j]) {
                storage_count = 0;
                return;
            }
        }
    }
}

/* for testing purposes */
void message_reset(void)
{
    uint8_t i;
    for (i = 0; i < MAX_MESSAGES; i++) {
        storage_nodes[i] = 0;
    }
    storage_count = 0;
}
