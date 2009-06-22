#include "text.h"
#include "lcd.h"

// TODO: Test
// TODO: Think about storing bit matrix data externally and not within 8kb flash 

void lcd_draw_char(unsigned char c)
{
    // Insert a little space
    lcd_write(0x0);
    
    // Characters are represented as bit matrices. There are no lower-case
    // characters.
    //
    // Width: 4-bit, usually, but don't rely on this!
    // Height: 5-bit
    // Padding: top 1-bit, bottom 1-bit (by convention)
    // 
    // In order to convert the hex representation of a character back to a
    // readable matrix representation, write the hex values as column vectors
    // in binary representation. Example:
    //
    // F: 
    //  lcd_write(0x7c);
    //  lcd_write(0x50);
    //  lcd_write(0x50);
    //  lcd_write(0x40);
    //
    //      <-->
    //
    //  0 0 0 0
    //  x x x x
    //  7 5 5 4
    //  c 0 0 0
    //
    //      <-->
    //
    //  0 0 0 0
    //  1 1 1 1
    //  1 0 0 0
    //  1 1 1 1
    //  1 0 0 0
    //  1 0 0 0
    //  0 0 0 0
    //  0 0 0 0
    //
    //      <-->
    //
    //         
    //  1 1 1 1
    //  1      
    //  1 1 1 1
    //  1      
    //  1      
    //         
    //         
    
    switch (c) 
    {
        case 'a':
        case 'A':
            lcd_write(0x3c);
            lcd_write(0x50);
            lcd_write(0x50);
            lcd_write(0x3c);
            break;
        case 'b':
        case 'B':
            lcd_write(0x7c);
            lcd_write(0x54);
            lcd_write(0x54);
            lcd_write(0x2c);
            break;
        case 'c':
        case 'C':
            lcd_write(0x38);
            lcd_write(0x44);
            lcd_write(0x44);
            lcd_write(0x44);
            break;
        case 'd':
        case 'D':
            lcd_write(0x7c);
            lcd_write(0x44);
            lcd_write(0x44);
            lcd_write(0x38);
            break;
        case 'e':
        case 'E':
            lcd_write(0x7c);
            lcd_write(0x54);
            lcd_write(0x54);
            lcd_write(0x44);
            break;
        case 'f':
        case 'F':
            lcd_write(0x7c);
            lcd_write(0x50);
            lcd_write(0x50);
            lcd_write(0x40);
            break;
        case '1':
            lcd_write(0x10);
            lcd_write(0x20);
            lcd_write(0x7c);
            break;
        case '2':
            lcd_write(0x24);
            lcd_write(0x4c);
            lcd_write(0x54);
            lcd_write(0x24);
            break;
        case '3':
            lcd_write(0x44);
            lcd_write(0x54);
            lcd_write(0x54);
            lcd_write(0x38);
            break;
    }
}
