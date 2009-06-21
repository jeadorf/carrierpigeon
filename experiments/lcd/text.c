#include <text.h>
#include <lcd.h>

// TODO: Test
// TODO: Think about storing bit matrix data externally and not within 8kb flash 

void lcd_draw_char(unsigned char c)
{
    // insert a little space
    lcd_write(0x0);
    
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
            lcd_write(0x28);
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
            lcd_write(0x7d);
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
            lcd_write(0x20);
            lcd_write(0x40);
            lcd_write(0x5c);
            lcd_write(0x20);
            break;
        case '3':
            lcd_write(0x44);
            lcd_write(0x54);
            lcd_write(0x54);
            lcd_write(0x38);
            break;
    }
}
