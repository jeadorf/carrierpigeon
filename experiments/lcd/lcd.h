#include <avr/io.h> 
#include <util/delay.h>
#include "main.h"

void lcd_control(unsigned char control);
void lcd_write(char data);
void lcd_init(void);

// Port Pin Assignments
// PA0-7 -> D0-7

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

