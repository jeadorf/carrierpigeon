#include <avr/io.h> 
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define set_bit(port, bit)   ((port) |= (1 << (bit)))
#define clear_bit(port, bit) ((port) &= ~(1 << (bit)))

