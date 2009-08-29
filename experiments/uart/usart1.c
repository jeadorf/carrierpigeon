// AVR306: Using the AVR UART in C
// Routines for polled USART
// Last modified: 02-06-21
// Modified by: AR

/* Includes */
#include <avr/io.h>
#include "text.h"
#include "eeprom.h"
#include <stdio.h>
#include "global.h"

/* Prototypes */
void usart_init(unsigned int baud_register_value);
unsigned char usart_receive(void);
void usart_transmit(unsigned char data);

int main(void)
{
    int i = 0;

    lcd_init();
    lcd_clear();

    usart_init(51);

    lcd_set_page(0);
    lcd_set_column(10);
    lcd_draw_char('!');
    while (1)
    {
        unsigned char c = usart_receive();
        eeprom_write(i, c);
        i++;
        lcd_draw_char(c);
        /*unsigned char result[4];
           sprintf(result, "%3d", c);
           for (i = 0; i < 3; i++) 
           {
           lcd_draw_char(result[i]);
           } */
    }

    /* 
       while (1)
       {
       USART0_Transmit( USART0_Receive() );
       } */

    return 0;
}

/* Initialize UART */
void usart_init(unsigned int baud_register_value)
{
    // select UBRRH
    clear_bit(UCSRC, URSEL);

    /* Set the baud rate - note that register value and baud rate of
       target are not the same but can be calculated from each other */
    UBRRH = (unsigned char) (baud_register_value >> 8);
    UBRRL = (unsigned char) baud_register_value;

    /* Enable UART receiver and transmitter */
    set_bit(UCSRB, RXEN);
    set_bit(UCSRB, TXEN);

    // do not use double speed mode
    clear_bit(UCSRA, U2X);

    /* probably does not work because UCSRC must be set at once
       because of URSEL bit 
       // select UCSRC
       set_bit(UCSRC, URSEL);

       // 8 data bits
       set_bit(UCSRC, UCSZ2);
       clear_bit(UCSRC, UCSZ1);
       clear_bit(UCSRC, UCSZ0);

       // no parity bit
       clear_bit(UCSRC, UPM1);

       // only one stop bit
       clear_bit(UCSRC, USBS);
     */


    UCSRC = (1 << URSEL) | (3 << UCSZ0);

    //UCSRC = (1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);              //For devices with Extended IO
    //UCSR0C = (1<<URSEL)|(1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);   //For devices without Extended IO
}


/* Read and write functions */
unsigned char usart_receive(void)
{
    /* Wait for incomming data */
    while (!(UCSRA & (1 << RXC)))
        ;
    /* Return the data */
    return UDR;
}

void usart_transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSRA & (1 << UDRE)))
        ;
    /* Start transmittion */
    UDR = data;
}
