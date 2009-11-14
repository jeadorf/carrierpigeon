#include "eeprom.h"

/* 
 * Fills the first 256 bytes of the EEPROM with numbers from
 * 0 to 255 and then copy all bytes to the last 256 bytes.
 * If you want to verify that the operation succeeded, simply
 * extract the EEPROM memory using avrdude:
 *
 * avrdude -c avrisp2 -P usb -p atmega8515 -U eeprom:r:eeprom_memory.d:d       
 */
int main(void)
{
    int i;
    for (i = 0; i < 256; i++) {
        eeprom_write(i, (unsigned char) i);
    }

    unsigned char c;
    for (i = 256; i < 512; i++) {
        c = eeprom_read(i - 256);
        eeprom_write(i, c);
    }

    return 0;
}
