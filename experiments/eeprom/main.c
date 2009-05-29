// TODO: Does stdbool allow to use true and false as boolean constants?
#include <stdbool.h>
#include "main.h"
#include "eeprom.h"

int main(void)
{
    /* Fill the first 256 bytes of the EEPROM with numbers from 0 to 255 and
       then copy all bytes to the last 256 bytes. If you want to verify that
       the operation succeeded, you can extract EEPROM memory using
       
       avrdude -c avrisp2 -P usb -p atmega8515 -U eeprom:r:eeprom_memory.d:d
       
       The produced file contains a decimal representation of the EEPROM memory
       content. 
    */
       
    int i;
    for (i = 0; i < 256; i++)  
    {
        eeprom_write(i, (unsigned char) i);
    }
        
    unsigned char c;
    for (i = 256; i < 512; i++)  
    {
        c = eeprom_read(i - 256);
        eeprom_write(i, c);
    }
    
    return 0;
}
