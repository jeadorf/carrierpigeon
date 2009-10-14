/*
 * This header file contains all global definitions that are used both within
 * the main program as well as by the library routines.
 */

/* Use this macro to set a single bit. To set more than one bit, use the
 * following pattern: PORTX |= (1 << PX2) | (1 << PX4) | (1 << PX7); */
#define set_bit(port, bit)   ((port) |= (1 << (bit)))

/* Use this macro to clear a single bit. To clear more than one bit, use the
 * following pattern: PORTX &= ~(1 << PX2) | (1 << PX4) | (1 << PX7); */
#define clear_bit(port, bit) ((port) &= ~(1 << (bit)))

#define UART_BAUD_RATE 19200

/** This constant defines the size of the global buffer. Since
 * this buffer is used for several different purposes, you have
 * to be extremely careful and look at each purpose before changing
 * the size of this buffer. */
#define GLOBAL_BUFFER_SIZE 32 

