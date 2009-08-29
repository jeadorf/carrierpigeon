#include <avr/io.h>
#include <avr/interrupt.h>

// USART buffer defines
// 2,4,8,16,32,64,128 or 256 bytes
#define USART_RX_BUFFER_SIZE 128
// 2,4,8,16,32,64,128 or 256 bytes
#define USART_TX_BUFFER_SIZE 128
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )
#define USART_TX_BUFFER_MASK ( USART_TX_BUFFER_SIZE - 1 )
#if ( USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( USART_TX_BUFFER_SIZE & USART_TX_BUFFER_MASK )
#error TX buffer size is not a power of 2
#endif

// static variables
static unsigned char USART_RxBuf[USART_RX_BUFFER_SIZE];
static volatile unsigned char USART_RxHead;
static volatile unsigned char USART_RxTail;
static unsigned char USART_TxBuf[USART_TX_BUFFER_SIZE];
static volatile unsigned char USART_TxHead;
static volatile unsigned char USART_TxTail;

// prototypes
void USART_Init(unsigned int baudrate);
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char data);

// initialize USART
void USART_Init(unsigned int baudrate)
{
    unsigned char x;

    // set the baud rate
    UBRRL = (unsigned char) baudrate;

    // Enable UART receiver and transmitter
    UCSRB = ((1 << RXCIE) | (1 << RXEN) | (1 << TXEN));

    // set frame format: 8 data 2stop
    // for devices with Extended IO
    UCSRC = (1 << USBS) | (1 << UCSZ1) | (1 << UCSZ0);
    // for devices without Extended IO      
    //UCSRC = (1 << URSEL) |(1 << USBS )| (1 << UCSZ1)| (1 << UCSZ0);

    // flush receive buffer
    x = 0;

    USART_RxTail = x;
    USART_RxHead = x;
    USART_TxTail = x;
    USART_TxHead = x;
}

// interrupt handlers
// RX interrupt
ISR(USART_RX_vect)
{
    unsigned char data;
    unsigned char tmphead;

    // read the received data
    data = UDR;
    // calculate buffer index
    tmphead = (USART_RxHead + 1) & USART_RX_BUFFER_MASK;
    // store new index
    USART_RxHead = tmphead;

    if (tmphead == USART_RxTail)
    {
        // ERROR! receive buffer overflow
    }

    // store received data in buffer
    USART_RxBuf[tmphead] = data;
}

// TX interrupt
ISR(USART_UDRE_vect)
{
    unsigned char tmptail;

    // check if all data is transmitted
    if (USART_TxHead != USART_TxTail)
    {
        // calculate buffer index
        tmptail = (USART_TxTail + 1) & USART_TX_BUFFER_MASK;
        // store new index
        USART_TxTail = tmptail;

        // start transmition
        UDR = USART_TxBuf[tmptail];
    }
    else
    {
        // disable UDRE interrupt
        UCSRB &= ~(1 << UDRIE);
    }
}

// read and write functions
unsigned char USART_Receive(void)
{
    unsigned char tmptail;

    // wait for incoming data
    while (USART_RxHead == USART_RxTail)
        ;

    // calculate buffer index
    tmptail = (USART_RxTail + 1) & USART_RX_BUFFER_MASK;

    // store new index
    USART_RxTail = tmptail;

    // return data
    return USART_RxBuf[tmptail];
}

void USART_Transmit(unsigned char data)
{
    unsigned char tmphead;
    // calculate buffer index
    tmphead = (USART_TxHead + 1) & USART_TX_BUFFER_MASK;
    // wait for free space in buffer
    while (tmphead == USART_TxTail)
        ;

    // store data in buffer
    USART_TxBuf[tmphead] = data;
    // store new index
    USART_TxHead = tmphead;

    // enable UDRE interrupt
    UCSRB |= (1 << UDRIE);
}

unsigned char DataInReceiveBuffer(void)
{
    // return 0 (false) if the receive buffer is empty
    return (USART_RxHead != USART_RxTail);
}
