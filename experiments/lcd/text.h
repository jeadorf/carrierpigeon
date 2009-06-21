/*
 * Prints a character at the current page and column. The column address will be
 * incremented. Subsequent calls to this function print a sequence of characters
 * on the display.
 *
 * There is no line-wrapping, and no EOL detection. This function does not do
 * anything else than drawing the bitmatrix that represents the given character.
 *
 * c  --  A character value such as c = 'a'. For information regarding the
 *        supported characters, refer to the implementation.  
 */
void lcd_draw_char(unsigned char c);
