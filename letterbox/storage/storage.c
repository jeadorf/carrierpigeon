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
#include "eeprom.h"

// List of nodes that point to message blocks. Zero marks a free slot
// in the list. The first zero denotes the end of the list. A value
// n denotes that the message is actually stored in the n-th memory
// block (counting begins with 1)
unsigned char storage_nodes[4] = { 0, 0, 0, 0 };
// Points to the next free slot in storage_nodes 
unsigned char storage_count = 0;
// Points to the currently opened message entry
int block = -1;
// Points to the current read/write position
int pos = -1;

/** use the globally available buffer in order to
 * reduce memory consumption */
extern char global_buffer[];

bool storage_new(void)
{
    bool found_free;
    if (!storage_is_full()) {
        // Find free memory block
        // Check all blocks, do not do this by reading the
        // EEPROM message status bit, it should be faster this way.
        for (block = 1; block <= MAX_MESSAGES; block++) {
            int i;
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

int storage_write(char* buf)
{
    int i = 0;
    int old = pos;
    while (buf[i] != '\0') {
        eeprom_write(pos, buf[i]);
        i++;
        pos++;
    }
    return pos - old;
}

bool storage_open(int msg_num)
{
    if (msg_num < storage_message_count()) {
        block = storage_nodes[msg_num]; 
        pos = (block - 1) * MESSAGE_SIZE;
        pos += MESSAGE_RESERVED_SIZE + 1;
        return true;
    } else {
        return false;
    }
}

char storage_read(void)
{
    return eeprom_read(pos++);
}

void storage_close(void)
{
    if (pos - (block - 1) * MESSAGE_SIZE < MESSAGE_SIZE) {
        eeprom_write(pos, '\0'); 
    }

    block = -1;
    pos = -1;
}

bool storage_is_empty(void)
{
    return storage_count == 0;
}

bool storage_is_full(void)
{
    return storage_count == MAX_MESSAGES;
}

void _storage_save_message_to_eeprom(unsigned char block)
{
    char c;
    eeprom_write((block - 1) * MESSAGE_SIZE, STATE_NEW);
    unsigned int text_addr = (block - 1) * MESSAGE_SIZE + MESSAGE_RESERVED_SIZE + 1;
    int i;
    for (i = 0; i < MESSAGE_TEXT_LENGTH; i++)
    {
        c = global_buffer[i];
        eeprom_write(text_addr + i, c);
        if (global_buffer[i] == '\0') {
            break;
        }
    }
}

bool storage_save_message(void)
{
    bool found_free;
    if (storage_is_full()) {
        return false;
    } else {
        // Find free memory block
        int block;
        // Check all blocks, do not do this by reading the
        // EEPROM message status bit, it should be faster this way.
        for (block = 1; block <= MAX_MESSAGES; block++) {
            int i;
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
                // Write message to the eeprom memory
                _storage_save_message_to_eeprom(block);
                storage_count++;
                break;
            }
        }
        return true;
    }
}

unsigned char storage_get_state(unsigned int message_num)
{
    if (message_num < storage_count)
    {
        unsigned char block = storage_nodes[message_num];
        unsigned char mem_addr = (block - 1) * MESSAGE_SIZE; 
        return eeprom_read(mem_addr);
    }

    return STATE_EMPTY;
}

bool storage_load_message(unsigned int message_num)
{
    if (message_num < storage_count) {
        unsigned char block = storage_nodes[message_num];
        unsigned int text_addr = (block - 1) * MESSAGE_SIZE + MESSAGE_RESERVED_SIZE + 1;
        int i;
        char c;
        for (i = 0; i < MESSAGE_TEXT_LENGTH; i++) {
            c = eeprom_read(text_addr + i);
            global_buffer[i] = c;
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
    if (message_num < storage_count) {
        // Shift all message pointers left, thus removing one pointer.
        int i;
        for (i = message_num + 1; i < storage_count; i++) {
            storage_nodes[i - 1] = storage_nodes[i];
        }
        storage_nodes[storage_count - 1] = 0;
        storage_count--;
        return true;
    } else {
        return false;
    }
}

char* storage_get_buffer(void)
{
    return global_buffer;
}

unsigned int storage_message_count(void)
{
    return storage_count;
}

/* for testing purposes */
void storage_reset(void)
{
    int i;
    for (i = 0; i < MAX_MESSAGES; i++) {
        storage_nodes[i] = 0;
    }
    storage_count = 0;
}

