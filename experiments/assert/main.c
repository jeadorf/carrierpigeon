#include <stdbool.h>
#include "lcd.h"
#include "assert.h"

int main(void)
{
    lcd_init();
    lcd_clear();
    assert_true("Expected this to be true", false);
    return 0;
}
