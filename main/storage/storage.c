/*
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
 * reorganized. 
 */

#include "storage.h"
#include "global.h"
#include "eeprom.h"
#include "stddef.h"

// This is where the first node resides in memory
#define NODE_STORAGE_OFFSET 0
// This is where the first message memory block begins
#define MESSAGE_STORAGE_OFFSET 7
#define MAX_MESSAGE_COUNT 4
#define MESSAGE_SIZE 120
#define MAX_MESSAGE_TEXT_LENGTH 112

void _storage_save_message_at(unsigned char mem_addr, unsigned char *text)
{
    eeprom_write(mem_addr, STATE_NEW);
    // Skip seven reserved bytes
    mem_addr += 7;
    // TODO: Catch overflow 
    for (; *text != '\0'; mem_addr++, text++)
    {
        eeprom_write(mem_addr, *text);
    }
}

bool storage_save_message(unsigned char state, unsigned char *text)
{
    // 1. Find the next free node
    

    /* int i;
    for (i = 0; i < MAX_MESSAGE_COUNT; i++)
    {
        unsigned char b = eeprom_read(NODE_STORAGE_OFFSET + i);
        if (b == )
        {
            unsigned char mem_addr =
                MESSAGE_STORAGE_OFFSET + i * MESSAGE_SIZE;
            _storage_save_message_at(mem_addr, text);
            return true;
        }
    }

    return false; */
}

unsigned char storage_get_state(unsigned int message_number)
{
    if (message_number < storage_message_count())
    {
        unsigned char mem_addr =
            MESSAGE_STORAGE_OFFSET + (message_number - 1) * MESSAGE_SIZE;
        return eeprom_read(mem_addr);
    }

    return STATE_EMPTY;
}

unsigned char *storage_get_text(unsigned int message_number)
{
    int i;
    unsigned char *c;
    if (message_number < storage_message_count())
    {
        unsigned char mem_addr =
            MESSAGE_STORAGE_OFFSET + (message_number - 1) * MESSAGE_SIZE;
        mem_addr += 7;

        for (i = 0; i < MAX_MESSAGE_TEXT_LENGTH - 1 && *c != '\0'; i++, c++)
        {
            *c = eeprom_read(mem_addr);
        }
        *(c++) = '\0';
        return c;
    }

    return NULL;
}

unsigned int storage_message_count()
{
    int count = 0;
    int mem_addr = MESSAGE_STORAGE_OFFSET;
    unsigned char s;
    while (count < MAX_MESSAGE_COUNT
           && (s = eeprom_read(mem_addr)) != STATE_EMPTY)
    {
        count++;
        mem_addr += MESSAGE_SIZE;
    }
    return count;
}
