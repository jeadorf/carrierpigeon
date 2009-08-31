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

#include "storage.h"
#include "global.h"
#include "eeprom.h"
#include "stddef.h"
#include "stdlib.h"

// avoid magic numbers
#define MAX_MESSAGE_COUNT 4
#define MESSAGE_SIZE 120
#define MAX_MESSAGE_TEXT_LENGTH 112
#define MESSAGE_RESERVED_SIZE 7

// List of nodes that point to message blocks. Zero marks a free slot
// in the list. The first zero denotes the end of the list. A value
// n denotes that the message is actually stored in the n-th memory
// block (counting begins with 1)
unsigned char storage_nodes[4] = { 0, 0, 0, 0 };
// Points to the next free slot in storage_nodes 
unsigned char message_count = 0;
// A buffer that is used for IO operations
unsigned char storage_message_buffer[MAX_MESSAGE_TEXT_LENGTH];

bool storage_is_empty(void)
{
    return message_count == 0;
}

bool storage_is_full(void)
{
    return message_count == MAX_MESSAGE_COUNT;
}

void _storage_save_message_to_eeprom(unsigned char block)
{
    eeprom_write((block - 1) * MESSAGE_SIZE, STATE_NEW);
    unsigned int text_addr = (block - 1) * MESSAGE_SIZE + MESSAGE_RESERVED_SIZE + 1;
    int i;
    for (i = 0; i < MAX_MESSAGE_TEXT_LENGTH; i++)
    {
        eeprom_write(text_addr + i, storage_message_buffer[i]);
        if (storage_message_buffer[i] == '\0') {
            break;
        }
    }
}

bool storage_save_message(void)
{
    if (storage_is_full()) {
        return false;
    } else {
        // Find free memory block
        int block;
        outer:
        // Check all blocks, do not do this by reading the
        // EEPROM message status bit, it should be faster this way.
        for (block = 1; block <= MAX_MESSAGE_COUNT; block++) {
            int i;
            bool free = true;
            // Look whether one node points to this block.
            for (i = 0; i < MAX_MESSAGE_COUNT; i++) {
                if (storage_nodes[i] == block) {
                    free = false;
                    break;
                }
            }
            if (free) {
                // Found a free memory block
                storage_nodes[message_count] = block;
                // Write message to the eeprom memory
                _storage_save_message_to_eeprom(block);
                message_count++;
                break;
            }
        }
        return true;
    }
}

unsigned char storage_get_state(unsigned int message_num)
{
    if (message_num < message_count)
    {
        unsigned char block = storage_nodes[message_num];
        unsigned char mem_addr = (block - 1) * MESSAGE_SIZE; 
        return eeprom_read(mem_addr);
    }

    return STATE_EMPTY;
}

bool storage_get_text(unsigned int message_num)
{
    if (message_num < message_count) {
        unsigned char block = storage_nodes[message_num];
        unsigned int text_addr = (block - 1) * MESSAGE_SIZE + MESSAGE_RESERVED_SIZE + 1;
        int i;
        char c;
        for (i = 0; i < MAX_MESSAGE_TEXT_LENGTH; i++) {
            c = eeprom_read(text_addr + i);
            storage_message_buffer[i] = c;
            if (c == '\0') {
                break;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool storage_delete_message(unsigned int message_num)
{
    if (message_num < message_count) {
        // Shift all message pointers left, thus removing one pointer.
        int i;
        for (i = message_num + 1; i < message_count; i++) {
            storage_nodes[i - 1] = storage_nodes[i];
        }
        storage_nodes[message_count - 1] = 0;
        message_count--;
        return true;
    } else {
        return false;
    }
}

unsigned char* storage_get_buffer(void)
{
    return storage_message_buffer;
}

unsigned int storage_message_count(void)
{
    return message_count;
}

void storage_reset(void)
{
    int i;
    for (i = 0; i < MAX_MESSAGE_COUNT; i++) {
        storage_nodes[i] = 0;
    }
    message_count = 0;
}

