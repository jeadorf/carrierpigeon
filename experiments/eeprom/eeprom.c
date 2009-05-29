/*
 * Utils for IO operations on the EEPROM. The atmega8515 provides an EEPROM with
 * 512 bytes, each of them can be addressed using the EEPROM address register
 * EEAR (two registers EEARH and EEARL).
 */

#include <avr/io.h>
#include "main.h"

/* TODO: Simplify set and read bit operations */
    
/*
 * Writes one byte to the EEPROM at the specified memory address.
 */
void eeprom_write(unsigned int address, unsigned char data)
{
    /* wait for completion of previous write */
    while (EECR & (1 << EEWE))
        ;
    
    /* set up address register */
    EEAR = address;    
    /* set up data register */
    EEDR = data;
    
    /* no write takes place unless the EMWE bit is set */
    EECR |= (1 << EEMWE);
    
    /* initiate eeprom write */
    EECR |= (1<<EEWE);
}

/*
 * Reads one byte from the EEPROM at the specified memory address.
 */
unsigned char eeprom_read(unsigned int address)
{
    /* wait for completion of previous write */
    while (EECR & (1 << EEWE))
        ;
        
    /* set up address register */
    EEAR = address;   
    /* start eeprom read */
    EECR |= (1 << EERE);
    
    return EEDR;
}
