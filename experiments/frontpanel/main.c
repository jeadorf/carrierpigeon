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

// milliseconds to wait
#define WAIT_TIME 250.0

int main(void)
{
  int n;
  // the lightness that is desired
  DDRB = NONE_LIGHT;
  
  while (1) {
    // turn on blue leds
    PORTB |= BLUE_LEDS;
    // wait
    _delay_ms(WAIT_TIME);

    // turn off blue leds, turn on red leds
    PORTB &= ~BLUE_LEDS;
    PORTB |= RED_LEDS;
    // wait
    _delay_ms(WAIT_TIME);

    // turn off red leds
    PORTB &= ~RED_LEDS;
  }

  return 0;
}
