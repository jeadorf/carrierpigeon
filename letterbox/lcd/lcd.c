#include "global.h"
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

int current_page = LCD_INIT_PAGE;
int current_column = LCD_INIT_COLUMN;

void lcd_light(unsigned char level)
{
    if (level >= 2)
    {
        set_bit(DDRE, LCD_BRIGHTNESS);
    }
    else
    {
        clear_bit(DDRE, LCD_BRIGHTNESS);
    }

    if (level > 0)
    {
        set_bit(PORTE, LCD_BRIGHTNESS);
    }
    else
    {
        clear_bit(PORTE, LCD_BRIGHTNESS);
    }
}

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

    clear_bit(PORTB, LCD_RS);
    clear_bit(PORTE, LCD_CS);
    clear_bit(PORTE, LCD_WR);
    set_bit(PORTB, LCD_RD);
    PORTA = control;
    set_bit(PORTE, LCD_WR);
    set_bit(PORTE, LCD_CS);
}

void lcd_write(unsigned char data)
{
    // writes arg0 to the currently selected column of the display
    // rs high for data

    set_bit(PORTB, LCD_RS);
    clear_bit(PORTE, LCD_CS);
    clear_bit(PORTE, LCD_WR);
    set_bit(PORTB, LCD_RD);
    PORTA = data;
    set_bit(PORTE, LCD_WR);
    set_bit(PORTE, LCD_CS);
}

void lcd_set_page(unsigned char pagenum)
{
    lcd_control(0xB0 + pagenum);
    current_page = pagenum;
}

void lcd_set_column(unsigned char colnum)
{
    // 4 most significant bits
    lcd_control(0x10 + (colnum >> 4));
    // 4 least significant bits
    lcd_control(0x00 + (colnum & 0x0F));
    current_column = colnum;
}

void lcd_clear(void)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        lcd_set_page(i);
        lcd_set_column(0);
        int j;
        for (j = 0; j < 132; j++)
        {
            lcd_write(0x00);
        }
    }
    lcd_set_column(LCD_INIT_COLUMN);
    lcd_set_page(LCD_INIT_PAGE);
}

/* reset the display */
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
    set_bit(DDRB, DDB3);
    set_bit(DDRC, DDC7);
    set_bit(DDRE, DDE0);
    set_bit(DDRE, DDE1);

    // with control lines all high except reset
    set_bit(PORTE, LCD_CS);
    clear_bit(PORTC, LCD_RST);

    set_bit(PORTB, LCD_RS);
    set_bit(PORTE, LCD_WR);
    set_bit(PORTB, LCD_RD);

    // startup delay of 100ms
    _delay_ms(100);
    // and rst high again
    set_bit(PORTC, LCD_RST);

    lcd_control(0xA2);          // <- Bias 1/9

    // both directions on reverse ("reverse" according to LCD datasheet)
    lcd_control(0xA1);          // <- ADC Direction L-R
    lcd_control(0xC8);          // <- SHL Direction 0-64

    lcd_control(0x25);          // <- Voltage ref
    lcd_control(0x81);          // <- Volume mode
    lcd_control(0x30);          // <- Volume set
    lcd_control(0x00);          // <- This probably isn't required here - it's set below
    lcd_control(0x2F);          // <- Vf, Vr, Vc on
    lcd_control(0x40);          // <- Initial display line
    lcd_control(0xA6);          // <- Normal display
    lcd_control(0xAF);          // <- turn display on

    lcd_control(0xB0);          // <- page address = 0
    lcd_control(0x10);          // <- column address high = 0
    lcd_control(0x00);          // <- column address low = 0

    lcd_set_column(LCD_INIT_COLUMN);
    lcd_set_page(LCD_INIT_PAGE);
}

