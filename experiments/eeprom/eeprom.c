#include "main.h"
#include "../../main/global.h"

void eeprom_write(unsigned int address, unsigned char data)
{
    // Wait for completion of previous write
    while (EECR & (1 << EEWE))
        ;
    
    // Set up address register
    EEAR = address;    
    // Set up data register
    EEDR = data;
    
    // No write takes place unless the EEMWE bit is set
    set(EECR, EEMWE); 
    
    /* Initiate eeprom write */
    set(EECR, EEWE); 
}

unsigned char eeprom_read(unsigned int address)
{
    // Wait for completion of previous write
    while (EECR & (1 << EEWE))
        ;
        
    // Set up address register
    EEAR = address;   
    
    // Start eeprom read
    set(EECR, EERE);
    
    return EEDR;
}
