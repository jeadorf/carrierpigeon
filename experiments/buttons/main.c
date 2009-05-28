#include "main.h"

#define LED_GREEN 0x10
#define PORTC_MASK 0x10
#define WAIT_TIME 250.0

#define SET_BIT(port, bit)   ((port) |= (1 << (bit)))
#define CLEAR_BIT(port, bit) ((port) &= ~(1 << (bit)))

void blink(void);
int get_key(void);

int get_key(void) 
{
	int keyvalue = 0;

	SET_BIT(DDRC, DDC3);
	SET_BIT(DDRC, DDC2);
	SET_BIT(DDRC, DDC1);
	SET_BIT(DDRC, DDC0);
	
	CLEAR_BIT(DDRD, DDD6);
	CLEAR_BIT(DDRD, DDD5);
	CLEAR_BIT(DDRD, DDD4);
	
	CLEAR_BIT(PORTC, PC3);
	SET_BIT(PORTC, PC2);
	SET_BIT(PORTC, PC1);
	SET_BIT(PORTC, PC0);
	
	if (bit_is_clear(PIND, PIND6)) {
		SET_BIT(PORTD, PD6);
		return 12;
	}
	
	return keyvalue;
}

int main(void)
{
	int key;
	
	while (1) {
		key = get_key();
		if (key == 12)
			blink();
	}
	
	return 0;
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
