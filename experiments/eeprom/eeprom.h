/*
 * Utils for IO operations on the EEPROM. The atmega8515 provides an EEPROM with
 * 512 bytes, each of them can be addressed using the 9 least significant bits
 * of the EEPROM address register EEAR (two registers EEARH and EEARL).
 */

/*
 * Writes exactly one byte to the EEPROM at the specified memory address.
 */
void eeprom_write(unsigned int address, unsigned char data);

/*
 * Reads exactly one byte from the EEPROM at the specified memory address.
 */
unsigned char eeprom_read(unsigned int address);

