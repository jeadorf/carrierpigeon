#include "main.h"
#include "buttons.h"

#define LED_GREEN 0x10
#define PORTC_MASK 0x10
#define WAIT_TIME 150.0

void blink(void);
void led_on(void);
void led_off(void);
void blink_times(int n);


int main(void)
{
	char key;
	
	while (1) {
		key = get_key();
		blink_times(key);
	}
	
	return 0;
}

void blink_times(int n) 
{
	int i;
	for (i = 0; i < n; i++) {
		blink();
	}
}

void led_on(void)
{
	SET_BIT(DDRC, DDC4);
	SET_BIT(PORTC, PC4);
}

void led_off(void)
{
	CLEAR_BIT(PORTC, PC4);
}


void blink(void)
{
	led_on();
	_delay_ms(WAIT_TIME);
	led_off();
	_delay_ms(WAIT_TIME);
}
