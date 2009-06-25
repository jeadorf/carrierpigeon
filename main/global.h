/*
 * This header file contains all global definitions that are used both within
 * the main program as well as by the library routines.
 */

/* Use this macro to set a single bit. To set more than one bit, use the
 * following pattern: PORTX |= (1 << PX2) | (1 << PX4) | (1 << PX7); */
#define set(port, bit)   ((port) |= (1 << (bit)))

/* Use this macro to clear a single bit. To clear more than one bit, use the
 * following pattern: PORTX &= ~(1 << PX2) | (1 << PX4) | (1 << PX7); */
#define clear(port, bit) ((port) &= ~(1 << (bit)))

