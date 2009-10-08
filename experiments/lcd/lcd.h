/* LCD library
 *
 * The LCD display is a 65x135 bit matrix. Bits cannot addressed directly.
 * Instead, the display is divided into 8 horizontal segments that are called
 * "pages". Each page consists of 132 columns. One column consists of 8 bit 
 * stacked vertically on top of another.
 */

#define LCD_INIT_COLUMN 0 
#define LCD_INIT_PAGE 7

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
void lcd_clear(void);

/* Sets brightness. 
 * level  --  0 (off), 1 (normal), 2 (bright)
 */
void lcd_light(unsigned char level);

/* level  --  0 - 63 */
void lcd_contrast(unsigned char level);

/* Sets the current page of the display RAM. The address
 * has to be in the range 0 - 7
 */
void lcd_set_page(unsigned char pagenum);

/** Sets the current column of the display RAM. */
void lcd_set_column(unsigned char colnum);

#define LCD_BRIGHTNESS PE2

// Port-Pin Assignments
// PA0-7 => D0-7

// chip select
#define LCD_CS  PE0
// chip reset
#define LCD_RST PC7
// data/control (aka A0)
#define LCD_RS  PB3
// write
#define LCD_WR  PE1
// read
#define LCD_RD  PB2

// line switches
// CS
#define set_lcd_cs() set_bit(PORTE, LCD_CS)
#define clear_lcd_cs() clear_bit(PORTE, LCD_CS)
// RST
#define set_lcd_rst() set_bit(PORTC, LCD_RST)
#define clear_lcd_rst() clear_bit(PORTC, LCD_RST)
// RS
#define set_lcd_rs() set_bit(PORTB, LCD_RS)
#define clear_lcd_rs() clear_bit(PORTB, LCD_RS)
// WR
#define set_lcd_wr() set_bit(PORTE, LCD_WR)
#define clear_lcd_wr() clear_bit(PORTE, LCD_WR)
// RD
#define set_lcd_rd() set_bit(PORTB, LCD_RD)
#define clear_lcd_rd() clear_bit(PORTB, LCD_RD)
