#include <avr/io.h>
#include <avr/interrupt.h>

/* UART Buffer Defines */
#define USART_RX_BUFFER_SIZE 128     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_TX_BUFFER_SIZE 128     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )
#define USART_TX_BUFFER_MASK ( USART_TX_BUFFER_SIZE - 1 )
#if ( USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK )
	#error RX buffer size is not a power of 2
#endif
#if ( USART_TX_BUFFER_SIZE & USART_TX_BUFFER_MASK )
	#error TX buffer size is not a power of 2
#endif

/* Static Variables */
static unsigned char USART_RxBuf[USART_RX_BUFFER_SIZE];
static volatile unsigned char USART_RxHead;
static volatile unsigned char USART_RxTail;
static unsigned char USART_TxBuf[USART_TX_BUFFER_SIZE];
static volatile unsigned char USART_TxHead;
static volatile unsigned char USART_TxTail;

/* Prototypes */
void USART_Init( unsigned int baudrate );
unsigned char USART_Receive( void );
void USART_Transmit( unsigned char data );

/* Initialize USART */
void USART_Init( unsigned int baudrate )
{
	unsigned char x;

	/* Set the baud rate */
	//UBRR0H = (unsigned char) (baudrate>>8);                  
	//UBRR0L = (unsigned char) baudrate;
	UBRRL = (unsigned char)baudrate;
	
	/* Enable UART receiver and transmitter */
	//UCSR0B = ( ( 1 << RXCIE0 ) | ( 1 << RXEN0 ) | ( 1 << TXEN0 ) ); 
	UCSRB = ( ( 1 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN ) ); 

	
	/* Set frame format: 8 data 2stop */
	//UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);              //For devices with Extended IO
	UCSRC = (1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);              //For devices with Extended IO
	
	//UCSR0C = (1<<URSEL)|(1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);   //For devices without Extended IO
	//UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);   //For devices without Extended IO
	
	/* Flush receive buffer */
	x = 0; 			    

	USART_RxTail = x;
	USART_RxHead = x;
	USART_TxTail = x;
	USART_TxHead = x;
}

/* Interrupt handlers */
//#pragma vector=USART0_RXC_vect 
//__interrupt void USART_RX_interrupt(void)
ISR(USART_RX_vect)
{
	unsigned char data;
	unsigned char tmphead;

	/* Read the received data */
	data = UDR;                 
	/* Calculate buffer index */
	tmphead = ( USART_RxHead + 1 ) & USART_RX_BUFFER_MASK;
	USART_RxHead = tmphead;      /* Store new index */

	if ( tmphead == USART_RxTail )
	{
		/* ERROR! Receive buffer overflow */
	}
	
	USART_RxBuf[tmphead] = data; /* Store received data in buffer */
}

//#pragma vector=USART0_UDRE_vect 
//__interrupt void USART_TX_interrupt( void )
// TX interrupt
ISR(USART_UDRE_vect)
{
	unsigned char tmptail;

	/* Check if all data is transmitted */
	if ( USART_TxHead != USART_TxTail )
	{
		/* Calculate buffer index */
		tmptail = ( USART_TxTail + 1 ) & USART_TX_BUFFER_MASK;
		USART_TxTail = tmptail;      /* Store new index */
	
		UDR = USART_TxBuf[tmptail];  /* Start transmition */
	}
	else
	{
		UCSRB &= ~(1<<UDRIE);         /* Disable UDRE interrupt */
	}
}

/* Read and write functions */
unsigned char USART_Receive( void )
{
	unsigned char tmptail;
	
	while ( USART_RxHead == USART_RxTail )  /* Wait for incomming data */
		;
	tmptail = ( USART_RxTail + 1 ) & USART_RX_BUFFER_MASK;/* Calculate buffer index */
	
	USART_RxTail = tmptail;                /* Store new index */
	
	return USART_RxBuf[tmptail];           /* Return data */
}

void USART_Transmit( unsigned char data )
{
	unsigned char tmphead;
	/* Calculate buffer index */
	tmphead = ( USART_TxHead + 1 ) & USART_TX_BUFFER_MASK; /* Wait for free space in buffer */
	while ( tmphead == USART_TxTail );

	USART_TxBuf[tmphead] = data;           /* Store data in buffer */
	USART_TxHead = tmphead;                /* Store new index */

	UCSRB |= (1<<UDRIE);                    /* Enable UDRE interrupt */
}

unsigned char DataInReceiveBuffer( void )
{
	return ( USART_RxHead != USART_RxTail ); /* Return 0 (FALSE) if the receive buffer is empty */
}
