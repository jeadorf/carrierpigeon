#include <stdbool.h>
#include "main.h"

//***************************************************************************
//* Port Pin Assignments
//***************************************************************************

// pa0-7 ->   d0-7

// note that we have wired the display in 8080 mode
// so wr and rd rather than r/~w and e
#define   cs  PE0      /* chip select  */
#define   rst PC7      /* chip reset   */
#define   rs  PB3      /* data/control */
#define   wr  PE1      /* write        */
#define   rd  PB2      /* read         */
   
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

   PORTB &= ~_BV(rs);
   PORTE &= ~_BV(cs);
   PORTE &= ~_BV(wr);
   PORTA = control;
   PORTE |= _BV(wr);
   PORTE |= _BV(cs);
}
   
void lcd_write(char data)
{
   // writes arg0 to the currently selected column of the display
   // rs high for data

   PORTB |= _BV(rs);
   PORTE &= ~_BV(cs);
   PORTE &= ~_BV(wr);
   PORTC = data;
   PORTE |= _BV(wr);
   PORTE |= _BV(cs);
}

void lcd_init(void)
{
   // reset the display and clear it
   int q;
   
   // first, force a hardware reset
   // there isn't a spec for how long this requires
   // - it's suggested to tie it to the CPU reset -
   // but it has to wait until power is stable
   // so we'll delay for 100ms

   // set the ports to be outputs
   DDRA = 0xff;
   DDRB = 0xff;
   DDRC = 0xff;
   DDRD = 0xff;
   DDRE = 0xff;

   // with control lines all high except reset
   PORTC = (~(_BV(rst)));
   // startup delay of 100ms
   for (q=0; q<4; q++)
      _delay_loop_2(50000);
   // and rst high again
   PORTC = 0xff;

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

   // clear screen
   //lcd_cls();
      
   return;
} 

int main(void)
{
	lcd_init();

	return 0;
}
