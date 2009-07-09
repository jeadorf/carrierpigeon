#include <stdbool.h>
#include "main.h"
#include "led.h"

int main(void)
{
	// set the baudrate to 19,200 bps using a 3.6864MHz crystal
	USART_Init(11);
	// enable interrupts => enable UART interrupts
	sei();

	while (true)
	{
		/* Echo the received character */
		USART_Transmit(USART_Receive()); 
	}

	return 0;
}
