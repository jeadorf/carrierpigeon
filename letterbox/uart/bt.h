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
bool bt_readline(char* buf, int max_size);

