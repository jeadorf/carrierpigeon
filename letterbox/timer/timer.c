#include "global.h"
#include "led.h"
#include <avr/io.h>

int ticks = 0;

void timer_init(void) {
    // set prescale of timer to 1024
    set_bit(TCCR1B, CS12);
    set_bit(TCCR1B, CS10);
}

bool timer_poll(void) {
{
    if (TCNT1 >= 10800) {
        ticks++;
        TCNT1 = 0;
    }

    if (ticks >= TIMER_THRESHOLD) {
        ticks = 0;
        return true;
    } else {
        return false;
    }
}

