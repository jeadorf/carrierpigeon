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
uint8_t storage_nodes[4] = { 0, 0, 0, 0 };
// Memory state: Points to the next free slot in storage_nodes 
uint8_t storage_count = 0;

// Streaming state: Points to the currently opened message entry
uint8_t block;
// Streaming state: Points to the current read/write position
uint16_t pos;

bool message_new(void)
{
    uint8_t i;
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
                pos = (block - 1) * MESSAGE_SIZE;

                // Write status byte and skip reserved
                // message part
                eeprom_write(pos, STATE_NEW);
                pos += MESSAGE_RESERVED_SIZE + 1;
                break;
            }
        }
        return true;
    }

    block = -1;
    return false;    
}

uint8_t message_write(char* buf)
{
    uint8_t i = 0;
    uint8_t old = pos;
    while (buf[i] != '\0') {
        eeprom_write(pos, buf[i]);
        i++;
        pos++;
    }
    return pos - old;
}

bool message_open(uint8_t msg_num)
{
    if (msg_num < message_count()) {
        block = storage_nodes[msg_num]; 
        pos = (block - 1) * MESSAGE_SIZE;
        pos += MESSAGE_RESERVED_SIZE + 1;
        return true;
    } else {
        return false;
    }
}

char message_read(void)
{
    return eeprom_read(pos++);
}

void message_close(void)
{
    if (pos - (block - 1) * MESSAGE_SIZE < MESSAGE_SIZE) {
        eeprom_write(pos, '\0'); 
    }

    block = -1;
    pos = -1;
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
    if (msg_num < storage_count)
    {
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

/* for testing purposes */
void message_reset(void)
{
    uint8_t i;
    for (i = 0; i < MAX_MESSAGES; i++) {
        storage_nodes[i] = 0;
    }
    storage_count = 0;
}
