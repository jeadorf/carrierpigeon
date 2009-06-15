#include <avr/io.h> 
#include <util/delay.h>
#include "main.h"

void lcd_control(unsigned char control);
void lcd_write(char data);
void lcd_init(void);
void lcd_light(unsigned char level);

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
