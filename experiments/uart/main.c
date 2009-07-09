#include "main.h"
#include "led.h"

int main(void)
{
	USART_Init( 11 );   /* Set the baudrate to 19,200 bps using a 3.6864MHz crystal */

	sei();           /* Enable interrupts => enable UART interrupts */

	for( ; ; )        /* Forever */
	{
		USART_Transmit( USART_Receive() ); /* Echo the received character */
	}

	return 0;
}
