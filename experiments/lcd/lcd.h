/* LCD library
 *
 * The LCD display is a 65x135 bit matrix. Bits cannot addressed directly.
 * Instead, the display is divided into 8 horizontal segments that are called
 * "pages". Each page consists of 132 columns. One column consists of 8 bit 
 * stacked vertically on top of another.
 */

#include <avr/io.h> 
#include <util/delay.h>
#include "main.h"

/*
 * Writes a control byte to the display. See both lcd_init method and the 
 * display manual for further reference.
 */
void lcd_control(unsigned char control);

/*
 * Writes a byte to one display column. The write operation automatically 
 * increments the current column address.
 */
void lcd_write(unsigned char data);

/*
 * Sets display mode, contrast, brightness and makes sure the LCD is in a valid
 * state.
 */
void lcd_init(void);

/*
 * Wipes out all display RAM data.
 */
void lcd_clear(void)

/* Sets brightness. 
 * level  --  0 (off), 1 (normal), 2 (bright) */
void lcd_light(unsigned char level);

/* level  --  0 - 63 */
void lcd_contrast(unsigned char level)

/* Sets the current page of the display RAM. The address
 * num  --  0 - 7 */
void lcd_set_page(unsigned char num);

#define LCD_BRIGHTNESS PE2

// Port-Pin Assignments
// PA0-7 => D0-7

// chip select
#define LCD_CS  PE0
// chip reset
#define LCD_RST PC7
// data/control
#define LCD_RS  PB3
// write
#define LCD_WR  PE1
// read
#define LCD_RD  PB2

// line switches
// CS
#define SET_LCD_CS() SET_BIT(PORTE, LCD_CS)
#define CLEAR_LCD_CS() CLEAR_BIT(PORTE, LCD_CS)
// RST
#define SET_LCD_RST() SET_BIT(PORTC, LCD_RST)
#define CLEAR_LCD_RST() CLEAR_BIT(PORTC, LCD_RST)
// RS
#define SET_LCD_RS() SET_BIT(PORTB, LCD_RS)
#define CLEAR_LCD_RS() CLEAR_BIT(PORTB, LCD_RS)
// WR
#define SET_LCD_WR() SET_BIT(PORTE, LCD_WR)
#define CLEAR_LCD_WR() CLEAR_BIT(PORTE, LCD_WR)
// RD
#define SET_LCD_RD() SET_BIT(PORTB, LCD_RD)
#define CLEAR_LCD_RD() CLEAR_BIT(PORTB, LCD_RD)
