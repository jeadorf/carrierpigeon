#include <stdbool.h>
#include "main.h"
#include "commons.h"
#include "lcd.h"
#include "text.h"

int main(void)
{
    lcd_init();
    lcd_clear();

    lcd_set_page(0);
    lcd_set_column(10);

    unsigned char invert = 0xff;
    lcd_write(0xff);
    lcd_draw_char_masked('D', invert);
    lcd_draw_char_masked('E', invert);
    lcd_draw_char_masked('A', invert);
    lcd_draw_char_masked('D', invert);
    lcd_draw_char_masked('B', invert);
    lcd_draw_char_masked('E', invert);
    lcd_draw_char_masked('E', invert);
    lcd_draw_char_masked('F', invert);
    lcd_draw_char_masked('@', invert);
    lcd_draw_char_masked('1', invert);
    lcd_draw_char_masked('2', invert);
    lcd_draw_char_masked('3', invert);

    lcd_set_page(1);
    lcd_set_column(5);
    char i;
    for (i = 'A'; i <= 'Z'; i++)
    {
        lcd_draw_char(i);
    }

    lcd_set_page(7);
    lcd_set_column(5);
    lcd_display_string("Hello world!");
    lcd_display_char(' ');
    lcd_display_string("This should get split.");
    // after much text, it even wraps over and replaces text
    // at the top, so all new text can be seen.

    // lcd_display_string("A very long string that should wrap and overwrite the first characters within the display! This belongs to the long string, too! This, too! And this! And finally this!");

    return 0;
}
