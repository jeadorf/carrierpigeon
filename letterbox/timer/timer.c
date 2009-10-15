#include "timer.h"
#include "global.h"
#include "led.h"
#include <avr/io.h>
#include <stdbool.h>

int ticks = 0;
int timeout = 15;

void timer_init(void)
{
    // set prescale of timer to 1024
    set_bit(TCCR1B, CS12);
    set_bit(TCCR1B, CS10);
}

void timer_start(int new_timeout)
{
    timeout = new_timeout;
    ticks = 0;
    TCNT1 = 0;
}

bool timer_poll(void) 
{
    if (ticks >= timeout) {
        return true;
    } else {
       if (TCNT1 >= 10800) {
          ticks++;
          led_blink(LED_BLUE);
          TCNT1 = 0;
       }
       return false;
    }
}

