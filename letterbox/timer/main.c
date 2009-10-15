#include "timer.h"
#include "led.h"
#include <stdbool.h>

int main(void)
{
    timer_init();
    timer_start(5);
    while (!timer_poll()) {
    }
    while (true) {
        led_blink();
    }
}

