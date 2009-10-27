#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "buttons.h"
#include "led.h"

// rename to main to get this version
int main_hold(void)
{
    uint8_t key;

    while (true) {
        key = get_key();
        if (key != 0)
            led_on(LED_GREEN);
        else
            led_off(LED_GREEN);
    }

    return 0;
}

// rename to something else to deactivate
int main(void)
{
    char key;

    while (true) {
        key = get_key();
        led_blink_times(LED_GREEN, key);
    }

    return 0;
}
