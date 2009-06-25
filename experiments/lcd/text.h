/*
 * Prints a character at the current page and column. The column address will be
 * incremented. Subsequent calls to this function print a sequence of characters
 * on the display.
 *
 * There is no line-wrapping, and no EOL detection. This function does not do
 * anything else than drawing the 8x5-bit matrix that represents the given
 * character plus an extra 1-bit spacer column.
 *
 * c  --  A character value such as c = 'a'. For information regarding the
 *        supported characters, refer to the implementation.
 *
 * xor_mask  --  An 8-bit bitmask that is applied on each column of the bit
 *               matrix that represents the character. 0x00 leaves the matrix
 *               untouched, while 0xff inverts the matrix.  
 */
void lcd_draw_char_masked(unsigned char c, unsigned char xor_mask);

/* Equivalent to lcd_draw_char_masked with bitmask 0x00 */
void lcd_draw_char(unsigned char c);

/* Draws a string. Does not care about line-wrapping. */
void lcd_draw_string(unsigned char* s);

/* Maps a character to an index in the character table */
char lcd_char_to_index(unsigned char c);
