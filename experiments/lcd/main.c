#include "main.h"

int main(void)
{
	lcd_init();
	// fill whole display
	lcd_control(0xA5);

	// "turn on the bright lights" :)
	lcd_light(2);

	return 0;
}
