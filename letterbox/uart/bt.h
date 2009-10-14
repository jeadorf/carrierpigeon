#include <stdbool.h>

/*
 * Reads a line from the bluetooth module using the
 * serial interface. Calls to bt_readline are non-blocking.
 * Call bt_readline repeatedly. If it returns true, the
 * passed collecting buffer contains the received line
 * of text, or at least the first max_size characters.
 * As long as this function returns false, the line has not
 * been completely read and further calls are necessary in
 * order to complete.
 */
bool bt_readline_buffer(char* buf, int max_size);

/*
 * Almost equivalent to bt_readline_buffer, just that
 * this function writes directly to the EEPROM via
 * message_write. This function helps to avoid the
 * allocation of buffers in the SRAM.
 */
bool bt_readline_message(int max_size);

