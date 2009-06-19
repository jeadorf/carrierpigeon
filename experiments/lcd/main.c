#include <stdbool.h>
#include "main.h"
#include "lcd.h"

// the smaller the faster
#define DELAY_MS 20

int main(void)
{
    lcd_init();
    
    lcd_clear();
    
    lcd_set_page(5);
    lcd_set_column(0);
    
    int i;
    for (i = 0; i < 132; i++)
    {
	    lcd_write(i % 0xff);
    }
}

int _main(void)
{
	int contrast = 0;
	bool rising = true;

	lcd_init();
	// fill whole display
	lcd_control(0xA5);

	/* all-singing, all-dancing flashy animation
	 * which fades the display in and out, turning the
	 * lights off and on again
	 * (unknown whether it damages the HW or not)
	 */
	while (true)
	{
		lcd_contrast(contrast);
		_delay_ms(DELAY_MS);

		// make contrast move between 0-63
		if (rising)
		{
			if (contrast >= 63)
				rising = false;
			else
				contrast++;
		}
		else
		{
			if (contrast <= 0)
				rising = true;
			else
				contrast--;
		}

		// "turn on the bright lights" :)
		if (contrast >= 30)
			lcd_light(0);
		else if (contrast >= 0)
			lcd_light(1);
		else
			lcd_light(0);
	}

	return 0;
}
