#include <stdint.h>
#include <avr/io.h>

#define LED_GREEN PC4
#define LED_RED PC5
#define LED_BLUE PC6

#define WAIT_TIME 150.0

void led_init(void);
void led_blink(uint8_t led);
void led_on(uint8_t led);
void led_off(uint8_t led);
void led_blink_times(uint8_t led, int n);

