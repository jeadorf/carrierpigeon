#include <stdint.h>

/*
 * Returns an identifier for a button that is pressed. If button SWX is pressed,
 * return X (e.g. return 7 if SW7 is pressed). If no button is pressed, return
 * 0. After the button state is read all buttons will be reset.
 */
uint8_t get_key(void);

