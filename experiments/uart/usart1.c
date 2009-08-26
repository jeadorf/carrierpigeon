// AVR306: Using the AVR UART in C
// Routines for polled USART
// Last modified: 02-06-21
// Modified by: AR

/* Includes */
#include <avr/io.h>
#include "text.h"
#include "eeprom.h"
#include <stdio.h>

/* Prototypes */
void USART0_Init( unsigned int baudrate );
unsigned char USART0_Receive( void );
void USART0_Transmit( unsigned char data );


int main( void )
{
    int i = 0;

    lcd_init();
    lcd_clear();

    USART0_Init(51);

    lcd_set_page(0);
    lcd_set_column(10);
    lcd_draw_char('!');
    while (1)
    {
        unsigned char c = USART0_Receive();
        eeprom_write(i, c);
        i++;
        lcd_draw_char(c);
        /*unsigned char result[4];
        sprintf(result, "%3d", c);
        for (i = 0; i < 3; i++) 
        {
           lcd_draw_char(result[i]);
        }*/
    }

   /* 
    while (1)
	{
		USART0_Transmit( USART0_Receive() );
	}*/
    
    return 0;
}

/* Initialize UART */
void USART0_Init( unsigned int baudrate )
{
    /* Set the baud rate */
	UBRRH = (unsigned char) (baudrate>>8);                  
	UBRRL = (unsigned char) baudrate;

	/* Enable UART receiver and transmitter */
	UCSRB = ( ( 1 << RXEN ) | ( 1 << TXEN ) ); 

	/* Set frame format: 8 data 2stop */
	//UCSRC = (1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);              //For devices with Extended IO
	//UCSR0C = (1<<URSEL)|(1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);   //For devices without Extended IO
    UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
}


/* Read and write functions */
unsigned char USART0_Receive( void )
{
	/* Wait for incomming data */
	while ( !(UCSRA & (1<<RXC)) ) 	
		;			                
	/* Return the data */
	return UDR;
}

void USART0_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !(UCSRA & (1<<UDRE)) )
		; 			                
	/* Start transmittion */
	UDR = data; 			        
}
