#!/bin/sh
#
#
avrdude -P usb -c avrisp2 -p atmega8515 -U eeprom:r:eeprom_content_r:r
hexdump -C eeprom_content_r
