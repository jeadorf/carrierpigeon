#include "main.h"

int main(void)
{
	lcd_init();
	// fill whole display
	lcd_control(0xA5);

	return 0;
}
