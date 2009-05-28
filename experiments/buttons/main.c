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

	int col, othercol;
	for (col = PC3; col >= PC0; col--)
	{
		for (othercol = PC3; othercol >= PC0; othercol--) {
			if (othercol != col)
				SET_BIT(PORTC, othercol);
		}
		CLEAR_BIT(PORTC, col);
		// set DDRD6-4 to READ (needs to be in a loop, since DDR is strange)
		DDRD &= 0b10001111;
		
		//int row;
		//for (row = PD6; row >= PD4; row--) {
			
		if (bit_is_clear(PIND, PIND6)) {
			SET_BIT(PORTD, PD6);
			
			return (col * 3) + 3;
		}
		if (bit_is_clear(PIND, PIND5)) {
			SET_BIT(PORTD, PD5);
			return (col * 3) + 2;
		}
		if (bit_is_clear(PIND, PIND4)) {
			SET_BIT(PORTD, PD4);
			return (col * 3) + 1;
		}
	}

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
