#define UART_BAUD_RATE 19200

void reset_lcd(void);
char* uart_readline(void);
bool startswith(const char* str, const char* prefix);
