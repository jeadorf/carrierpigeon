#include <avr/io.h> 
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define SET_BIT(port, bit)   ((port) |= (1 << (bit)))
#define CLEAR_BIT(port, bit) ((port) &= ~(1 << (bit)))
