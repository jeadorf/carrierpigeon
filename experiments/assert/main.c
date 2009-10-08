#include <stdbool.h>
#include "led.h"
#include "assert.h"

int main(void)
{
    lcd_init();
    lcd_clear();
    lcd_set_column(5);
    lcd_set_page(7);
    assert_true("Expected this to be true", false);
    return 0;
}
