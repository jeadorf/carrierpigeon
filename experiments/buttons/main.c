#include "main.h"

#define LED_GREEN 0x10
#define PORTC_MASK 0x10
#define WAIT_TIME 250.0

void blink(void);

int main(void)
{
  int col2;
  int row1;

  DDRC = 0x1F;
  DDRD = 0x00;

  while (1) {
    PORTC |= 0x0E;
    if (!bit_is_set(PIND, 4)) {
      blink();
      DDRD = 0x10;
      PORTD |= 0x10;
      DDRD = 0x00; 
    }
  }

  return 0;
}

void blink(void)
{
  PORTC |= LED_GREEN;
  _delay_ms(WAIT_TIME);
  PORTC &= ~LED_GREEN;
  _delay_ms(WAIT_TIME);
}  
