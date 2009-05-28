#include "main.h"

#define LED_GREEN 0x10
#define PORTC_MASK 0x10
#define WAIT_TIME 150.0

#define SET_BIT(port, bit)   ((port) |= (1 << (bit)))
#define CLEAR_BIT(port, bit) ((port) &= ~(1 << (bit)))

void blink(void);
char get_key(void);
void blink_times(int n);

char get_key(void)
{
	// result: default 0 (no key pressed)
	char col, othercol, row, keyvalue = 0;

	// set DDRC3-0 to WRITE
	DDRC |= 0b00001111;

	// iterate over all 4 COLs
	for (col = PC3; col >= PC0; col--)
	{
		// set all cols to 1 except for the current col
		for (othercol = PC3; othercol >= PC0; othercol--) {
			if (othercol != col)
				SET_BIT(PORTC, othercol);
		}
		// set the current col to 0
		CLEAR_BIT(PORTC, col);
		// set DDRD6-4 to READ (needs to be in a loop, since DDR is strange)
		DDRD &= 0b10001111;

		// loop over all rows (from PD6 - PD4)
		for (row = PD6; row >= PD4; row--) {
			// if the bit is clear, the button is pressed
			if (bit_is_clear(PIND, row)) {
				// unpress it
				SET_BIT(PORTD, row);
			
				// return the value of the key
				// col * 3 because each col has 3 keys
				// row - 3 because the PDs have offset 3
				return (col * 3) + (row - 3);
			}
		}
	}

	return keyvalue;
}

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
