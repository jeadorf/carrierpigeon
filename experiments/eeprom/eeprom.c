#include <avr/io.h>
#include "main.h"

// TODO: Simplify set and read bit operations
// TODO: Are these procedures are predefined in the AVR libraries (probably not)

void eeprom_write(unsigned int address, unsigned char data)
{
    // wait for completion of previous write
    while (EECR & (1 << EEWE))
        ;
    
    // set up address register
    EEAR = address;    
    // set up data register
    EEDR = data;
    
    // no write takes place unless the EEMWE bit is set
    EECR |= (1 << EEMWE);
    
    /* initiate eeprom write */
    EECR |= (1 << EEWE);
}

unsigned char eeprom_read(unsigned int address)
{
    // wait for completion of previous write
    while (EECR & (1 << EEWE))
        ;
        
    // set up address register
    EEAR = address;   
    // start eeprom read
    EECR |= (1 << EERE);
    
    return EEDR;
}