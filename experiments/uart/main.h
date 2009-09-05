#define SET_BIT(port, bit)   ((port) |= (1 << (bit)))
#define CLEAR_BIT(port, bit) ((port) &= ~(1 << (bit)))

#define UART_BAUD_RATE 19200

void reset_lcd(void);
