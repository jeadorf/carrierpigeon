#include "main.h"

#define BLUE_LEDS 0x02
#define RED_LEDS 0x01

#define NONE_LIGHT 0x00
// RED_LIGHT 0x01
#define RED_LIGHT (ALL_DARK + 1)
// BLUE_LIGHT 0x02
#define BLUE_LIGHT (RED_LIGHT << 1)
// ALL_LIGHT 0x03
#define ALL_LIGHT (BLUE_LIGHT + 1)

int main(void)
{
  int n;
  // the lightness that is desired
  DDRB = NONE_LIGHT;
  
  while (1) {
    PORTB |= BLUE_LEDS;
    for(n=0; n < 10; n++)
      _delay_loop_2(60000);

    PORTB &= ~BLUE_LEDS;
    PORTB |= RED_LEDS;
    for(n=0; n < 10; n++)
      _delay_loop_2(60000);

    PORTB &= ~RED_LEDS;
    for(n=0; n < 10; n++)
      _delay_loop_2(60000);
  }

  return 0;
}
