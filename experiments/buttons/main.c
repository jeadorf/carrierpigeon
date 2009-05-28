#include <stdbool.h>
#include "main.h"
#include "buttons.h"
#include "led.h"

// rename to main to get this version
int main_hold(void)
{
	char key;
	
	while (true)
	{
		key = get_key();
		if (key != 0)
			led_on();
		else
			led_off();
	}
	
	return 0;
}

// rename to something else to deactivate
int main(void)
{
	char key;
	
	while (true)
	{
		key = get_key();
		blink_times(key);
	}
	
	return 0;
}
