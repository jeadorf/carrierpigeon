#include <stdbool.h>
#include "main.h"

int main(void)
{
	lcd_init();
	lcd_control(0xA5);

	return 0;
}
