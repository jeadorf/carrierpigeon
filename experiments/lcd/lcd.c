#include "lcd.h"

/* 0 (off), 1 (normal), 2 (bright) */
void lcd_light(unsigned char level)
{
	if (level >= 2)
	{
		SET_BIT(DDRE, LCD_BRIGHTNESS);
	}
	else
	{
		CLEAR_BIT(DDRE, LCD_BRIGHTNESS);
	}

	if (level > 0)
	{
		SET_BIT(PORTE, LCD_BRIGHTNESS);
	}
	else
	{
		CLEAR_BIT(PORTE, LCD_BRIGHTNESS);
	}
}

/* 0 - 63 */
void lcd_contrast(unsigned char level)
{
	// switch to volume mode
	lcd_control(0x81);
	// set volume
	lcd_control(level);
}

void lcd_control(unsigned char control)
{
	// write a control value to the KS0713
	// rst must remain high throughout
	// rs goes low
	// cs and wr go low
	// data written to port c
	// cs and wr go high
	// rs goes high (rs moves before and after
	// the other lines to meet timing constraints)
	
	PORTB &= ~_BV(LCD_RS);
	PORTE &= ~_BV(LCD_CS);
	PORTE &= ~_BV(LCD_WR);
	PORTB |= _BV(LCD_RD);
	PORTA = control;
	PORTE |= _BV(LCD_WR);
	PORTE |= _BV(LCD_CS);
}
   
void lcd_write(char data)
{
	// writes arg0 to the currently selected column of the display
	// rs high for data
	
	PORTB |= _BV(LCD_RS);
	PORTE &= ~_BV(LCD_CS);
	PORTE &= ~_BV(LCD_WR);
	PORTC = data;
	PORTE |= _BV(LCD_WR);
	PORTE |= _BV(LCD_CS);
}

/* reset the display and clear it */
void lcd_init(void)
{
	/* first, force a hardware reset
	 * there isn't a spec for how long this requires
	 * - it's suggested to tie it to the CPU reset -
	 * but it has to wait until power is stable
	 * so we'll delay for 100ms
	 */

	/* set-up the required ports to be outputs:
	 * all of PORTA (D0-7)
	 * one bit on PORTB (RS)
	 * one bit on PORTC (RST)
	 * two bits on PORTE (CS, WR)
	 */
	DDRA = 0xFF;
	SET_BIT(DDRB, DDB3);
	SET_BIT(DDRC, DDC7);
	SET_BIT(DDRE, DDE0);
	SET_BIT(DDRE, DDE1);
	
	// with control lines all high except reset
	SET_LCD_CS();
	CLEAR_LCD_RST();

	SET_LCD_RS();
	SET_LCD_WR();
	SET_LCD_RD();
	// startup delay of 100ms
	_delay_ms(100);
	// and rst high again
	SET_LCD_RST();
	
	lcd_control(0xA2);      // <- Bias 1/9
	lcd_control(0xA0);      // <- ADC Direction L-R
	lcd_control(0xC0);      // <- SHL Direction 0-64
	lcd_control(0x25);      // <- Voltage ref
	lcd_control(0x81);      // <- Volume mode
	lcd_control(0x20);      // <- Volume set
	lcd_control(0x00);      // <- This probably isn't required here - it's set below
	lcd_control(0x2F);      // <- Vf, Vr, Vc on
	lcd_control(0x40);      // <- Initial display line
	lcd_control(0xA7);      // <- Normal display
	lcd_control(0xAF);      // <- turn display on
	
	lcd_control(0xB0);      // <- page address = 0
	lcd_control(0x10);      // <- column address high = 0
	lcd_control(0x00);      // <- column address low = 0
} 

