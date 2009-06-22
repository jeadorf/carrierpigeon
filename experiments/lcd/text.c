#include "text.h"
#include "lcd.h"

/* 
 * Characters are represented as bit matrices. There are no lower-case
 * characters.
 *
 * Width: 4-bit, usually, but don't rely on this!
 * Height: 5-bit
 * Padding: top 1-bit, bottom 1-bit (by convention)
 * 
 * In order to convert the hex representation of a character back to a
 * readable matrix representation, write the hex values as column vectors
 * in binary representation. Example:
 *
 * F: 
 *  lcd_write(0x7c);
 *  lcd_write(0x50);
 *  lcd_write(0x50);
 *  lcd_write(0x40);
 *
 *      <-->
 *
 *  0 0 0 0
 *  x x x x
 *  7 5 5 4
 *  c 0 0 0
 *
 *      <-->
 *
 *  0 0 0 0
 *  1 1 1 1
 *  1 0 0 0
 *  1 1 1 1
 *  1 0 0 0
 *  1 0 0 0
 *  0 0 0 0
 *  0 0 0 0
 *
 *      <-->
 *
 *         
 *  1 1 1 1
 *  1      
 *  1 1 1 1
 *  1      
 *  1      
 */         
#define CHAR_TABLE_LENGTH 32

// some fun, configurable stuff
#define BOTTOM {0x04, 0x04, 0x7c, 0x04, 0x04}
#define UNIFAIL {0x7c, 0x44, 0x44, 0x7c, 0x00}
#define UNDEFINED_CHAR UNIFAIL

char characters[CHAR_TABLE_LENGTH][5] = {
    // A
    {0x3c, 0x50, 0x50, 0x3c, 0x00},
    // B
    {0x7c, 0x54, 0x54, 0x2c, 0x00},
    // C
    {0x38, 0x44, 0x44, 0x44, 0x00},
    // D
    {0x7c, 0x44, 0x44, 0x38, 0x00},
    // E
    {0x7c, 0x54, 0x54, 0x44, 0x00},
    // F
    {0x7c, 0x50, 0x50, 0x40, 0x00},
    // G
    {0x38, 0x44, 0x54, 0x5c},
    // H
    {0x7c, 0x10, 0x10, 0x7c},
    // I
    {0x44, 0x7c, 0x44, 0x00, 0x00},
    // J
    {0x48, 0x44, 0x44, 0x78},
    // K
    {0x7c, 0x10, 0x28, 0x44},
    // L
    {0x7c, 0x04, 0x04},
    // M
    {0x7c, 0x20, 0x10, 0x20, 0x7c},
    // N
    {0x7c, 0x20, 0x10, 0x08, 0x7c},
    // O
    {0x38, 0x44, 0x44, 0x38},
    // P
    {0x7c, 0x50, 0x50, 0x20},
    // Q
    {0x38, 0x44, 0x38, 0x04},
    // R
    {0x7c, 0x50, 0x58, 0x24},
    // S
    {0x24, 0x34, 0x54, 0x48},
    // T
    {0x00, 0x40, 0x7c, 0x40},
    // U
    {0x7c, 0x04, 0x04, 0x7c},
    // V
    {0x7c, 0x04, 0x08, 0x70},
    // W
    {0x78, 0x04, 0x18, 0x04, 0x78},
    // X
    {0x44, 0x38, 0x28, 0x44},
    // Y
    {0x70, 0x0d, 0x70},
    // Z
    {0x4c, 0x54, 0x54, 0x64},
    // 0
    UNDEFINED_CHAR,
    // 1
    {0x10, 0x20, 0x7c, 0x00, 0x00},
    // 2
    {0x24, 0x4c, 0x54, 0x24, 0x00},
    // 3
    {0x44, 0x54, 0x54, 0x38, 0x00},
    // 4
    {0x70, 0x08, 0x3c, 0x08, 0x00},
    // <undefined> at position CHAR_TABLE_LENGTH - 1
    // 
    UNDEFINED_CHAR
};

// TODO: Test
// TODO: Think about storing bit matrix data externally and not within 8kb flash 

void lcd_draw_char(unsigned char c)
{
    lcd_draw_char_masked(c, 0x00);
}

void lcd_draw_char_masked(unsigned char c, unsigned char xor_mask)
{
    char i;
    
    // Map character
    char j = lcd_char_to_index(c);
    for (i = 0; i < 5; i++)
    {
        lcd_write(characters[j][i] ^ xor_mask);
    }  
    // Insert a little space
    lcd_write(0x0 ^ xor_mask);
}

char lcd_char_to_index(unsigned char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c - 'A';
    } else if (c >= 'a' && c <= 'z') {
        return c - 'a';
    } else if (c >= '0' && c <= '9') {
        return c - '0' + 26;
    } else {
        return CHAR_TABLE_LENGTH - 1;
    } 
}
