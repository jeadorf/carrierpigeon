#include "main.h"

#define LED_GREEN 0x10
#define PORTC_MASK 0x10
#define WAIT_TIME 150.0

#define SET_BIT(port, bit)   ((port) |= (1 << (bit)))
#define CLEAR_BIT(port, bit) ((port) &= ~(1 << (bit)))

void blink(void);
int get_key(void);
void blink_times(int n);

int get_key(void) 
{
	// result: default 0 (no key pressed)
	int keyvalue = 0;

	// set DDRC3-0 to WRITE
	DDRC |= 0b00001111;
	// set DDRD6-4 to READ
	DDRD &= 0b10001111;
	
	// COL4
	CLEAR_BIT(PORTC, PC3);
	SET_BIT(PORTC, PC2);
	SET_BIT(PORTC, PC1);
	SET_BIT(PORTC, PC0);
	
	if (bit_is_clear(PIND, PIND6)) {
		SET_BIT(PORTD, PD6);
		return 12;
	}
	if (bit_is_clear(PIND, PIND5)) {
		SET_BIT(PORTD, PD5);
		return 11;
	}
	if (bit_is_clear(PIND, PIND4)) {
		SET_BIT(PORTD, PD4);
		return 10;
	}

	// COL3
	SET_BIT(PORTC, PC3);
	CLEAR_BIT(PORTC, PC2);
	SET_BIT(PORTC, PC1);
	SET_BIT(PORTC, PC0);

	if (bit_is_clear(PIND, PIND6)) {
		SET_BIT(PORTD, PD6);
		return 9;
	}
	if (bit_is_clear(PIND, PIND5)) {
		SET_BIT(PORTD, PD5);
		return 8;
	}
	if (bit_is_clear(PIND, PIND4)) {
		SET_BIT(PORTD, PD4);
		return 7;
	}
	
	// COL2
	SET_BIT(PORTC, PC3);
	SET_BIT(PORTC, PC2);
	CLEAR_BIT(PORTC, PC1);
	SET_BIT(PORTC, PC0);

	if (bit_is_clear(PIND, PIND6)) {
		SET_BIT(PORTD, PD6);
		return 6;
	}
	if (bit_is_clear(PIND, PIND5)) {
		SET_BIT(PORTD, PD5);
		return 5;
	}
	if (bit_is_clear(PIND, PIND4)) {
		SET_BIT(PORTD, PD4);
		return 4;
	}

	// COL1
	SET_BIT(PORTC, PC3);
	SET_BIT(PORTC, PC2);
	SET_BIT(PORTC, PC1);
	CLEAR_BIT(PORTC, PC0);

	if (bit_is_clear(PIND, PIND6)) {
		SET_BIT(PORTD, PD6);
		return 3;
	}
	if (bit_is_clear(PIND, PIND5)) {
		SET_BIT(PORTD, PD5);
		return 2;
	}
	if (bit_is_clear(PIND, PIND4)) {
		SET_BIT(PORTD, PD4);
		return 1;
	}

	SET_BIT(PORTC, PC0);
	
	return keyvalue;
}

int main(void)
{
	int key;
	
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
