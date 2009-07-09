#include "main.h"
#include "led.h"

int main(void)
{
	USART0_Init( 11 );   /* Set the baudrate to 19,200 bps using a 3.6864MHz crystal */

	_SEI();           /* Enable interrupts => enable UART interrupts */

	for( ; ; )        /* Forever */
	{
		USART0_Transmit( USART0_Receive() ); /* Echo the received character */
	}

	return 0;
}
