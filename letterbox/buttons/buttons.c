#include <avr/io.h>
#include <util/delay.h>
#include "commons.h"

char get_key(void)
{
    // result: default 0 (no key pressed)
    char col, othercol, row;
    char keyvalue = 0;

    // set DDRC3-0 to WRITE
    DDRC |= 0x0F;               //0b00001111;

    // iterate over all 4 COLs
    for (col = PC3; col >= PC0; col--) {
        // set all cols to 1 except for the current col
        for (othercol = PC3; othercol >= PC0; othercol--) {
            if (othercol != col)
                set_bit(PORTC, othercol);
        }
        // set the current col to 0
        clear_bit(PORTC, col);
        // set DDRD6-4 to READ (needs to be in a loop, since DDR is strange)
        DDRD &= 0x8F;           //0b10001111;

        // loop over all rows (from PD6 - PD4)
        for (row = PD6; row >= PD4; row--) {
            // if the bit is clear, the button is pressed
            if (bit_is_clear(PIND, row)) {
                // unpress it
                set_bit(PORTD, row);

                // return the value of the key
                // col * 3 because each col has 3 keys
                // row - 3 because the PDs have offset 3
                return (col * 3) + (row - 3);
            }
        }
    }

    return keyvalue;
}
