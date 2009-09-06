#include "text.h"
#include "lcd.h"

/* 
 * Characters are represented as bit matrices. There are no lower-case
 * characters. Characters are monospaced.
 *
 * Width:  5-bit
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
#define CHAR_TABLE_LENGTH 46

// define representation for undefined characters 
#define BOTTOM {0x04, 0x04, 0x7c, 0x04, 0x04}
#define UNIFAIL {0x7c, 0x44, 0x44, 0x7c, 0x00}
#define UNDEFINED_CHAR UNIFAIL

// be careful when changing character table, you will need to adjust both
// CHAR_TABLE_LENGTH and the mapping function lcd_char_to_index
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
    {0x38, 0x44, 0x54, 0x5c, 0x00},
    // H
    {0x7c, 0x10, 0x10, 0x7c, 0x00},
    // I
    {0x44, 0x7c, 0x44, 0x00, 0x00},
    // J
    {0x48, 0x44, 0x44, 0x78, 0x00},
    // K
    {0x7c, 0x10, 0x28, 0x44, 0x00},
    // L
    {0x00, 0x7c, 0x04, 0x04, 0x00},
    // M
    {0x7c, 0x20, 0x10, 0x20, 0x7c},
    // N
    {0x7c, 0x20, 0x10, 0x08, 0x7c},
    // O
    {0x38, 0x44, 0x44, 0x38, 0x00},
    // P
    {0x7c, 0x50, 0x50, 0x20, 0x00},
    // Q
    {0x38, 0x44, 0x38, 0x04, 0x00},
    // R
    {0x7c, 0x50, 0x58, 0x24, 0x00},
    // S
    {0x24, 0x34, 0x54, 0x48, 0x00},
    // T
    {0x00, 0x40, 0x7c, 0x40, 0x00},
    // U
    {0x7c, 0x04, 0x04, 0x7c, 0x00},
    // V
    {0x7c, 0x04, 0x08, 0x70, 0x00},
    // W
    {0x78, 0x04, 0x18, 0x04, 0x78},
    // X
    {0x44, 0x38, 0x28, 0x44, 0x00},
    // Y
    {0x00, 0x70, 0x0c, 0x70, 0x00},
    // Z
    {0x4c, 0x54, 0x54, 0x64, 0x00},
    // 0
    {0x38, 0x44, 0x44, 0x38, 0x00},
    // 1
    {0x00, 0x10, 0x20, 0x7c, 0x00},
    // 2
    {0x24, 0x4c, 0x54, 0x24, 0x00},
    // 3
    {0x44, 0x54, 0x54, 0x38, 0x00},
    // 4
    {0x70, 0x08, 0x3c, 0x08, 0x00},
    // 5
    {0x00, 0x74, 0x54, 0x5c, 0x00},
    // 6
    {0x00, 0x7c, 0x54, 0x5c, 0x00},
    // 7
    {0x00, 0x4c, 0x50, 0x60, 0x00},
    // 8
    {0x00, 0x7c, 0x54, 0x7c, 0x00},
    // 9
    {0x00, 0x74, 0x54, 0x7c, 0x00},
    // ?
    {0x00, 0x40, 0x54, 0x60, 0x00},
    // !
    {0x00, 0x00, 0x74, 0x00, 0x00},
    // .
    {0x00, 0x00, 0x04, 0x00, 0x00},
    // ,
    {0x00, 0x02, 0x04, 0x00, 0x00},
    // -
    {0x00, 0x10, 0x10, 0x10, 0x00},
    // +
    {0x00, 0x10, 0x38, 0x10, 0x00},
    // (SPACE)
    {0x00, 0x00, 0x00, 0x00, 0x00},
    // '
    {0x00, 0x00, 0x60, 0x00, 0x00},
    // "
    {0x00, 0x60, 0x00, 0x60, 0x00},
    // <undefined> at position CHAR_TABLE_LENGTH - 1
    // 
    UNDEFINED_CHAR
};

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

void lcd_display_char(unsigned char c)
{
    // static: preserve their values across function calls
    static int current_page = 7, current_column = 5;

    // split after column 110 has been reached
    if (current_column == 110)
    {
        // first place to put a character on is column 5
        current_column = 5;
        current_page--;
        lcd_set_page(current_page);
        lcd_set_column(current_column);
    }

    if (current_page < 0)
    {
        // the highest line is number 7, wraparound to that
        current_page = 7;
        lcd_set_page(current_page);
    }

    // any character has width 5, so add it to the counter
    current_column += 5;
    lcd_draw_char(c);
}

void lcd_display_string(const char* s)
{
    const char *p = s;
    while (*p != '\0')
    {
        lcd_display_char(*p);
        p++;
    }
}

/* maps a char to their index in the font definition array */
char lcd_char_to_index(unsigned char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c - 'A';
    }
    else if (c >= 'a' && c <= 'z')
    {
        return c - 'a';
    }
    else if (c >= '0' && c <= '9')
    {
        return c - '0' + 26;
    }
    else if (c == '?')
    {
        return 36;
    }
    else if (c == '!')
    {
        return 37;
    }
    else if (c == '.')
    {
        return 38;
    }
    else if (c == ',')
    {
        return 39;
    }
    else if (c == '-')
    {
        return 40;
    }
    else if (c == '+')
    {
        return 41;
    }
    else if (c == ' ')
    {
        return 42;
    }
    else if (c == '\'')
    {
        return 43;
    }
    else if (c == '"')
    {
        return 44;
    }
    else
    {
        return CHAR_TABLE_LENGTH - 1;
    }
}
