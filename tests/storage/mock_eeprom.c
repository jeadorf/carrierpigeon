/* Mocks the EEPROM library */
#include "stdio.h"

unsigned char eeprom_memory[512];

void eeprom_write(unsigned int address, unsigned char data) {
    eeprom_memory[address] = data;
}

unsigned char eeprom_read(unsigned int address) {
    return eeprom_memory[address];
}

void mock_eeprom_print(void)
{
    int i;
    int c;
    for (i = 0; i < 512; i++) {
        c = eeprom_memory[i];
        if (c < 10 || c > 140) {
            printf("-");
        } else {
            printf("%c", c);
        }
        if ((i+1) % 32 == 0) {
            printf("\n");
        }
    }
}

void mock_eeprom_reset(void)
{
    int i;
    for (i = 0; i < 512; i++) {
        eeprom_memory[i] = 0;
    }
}

