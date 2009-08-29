#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/* Ports:

PA0.O:7 LCD_D0:7

PB0.O: LEDA_BRIGHTNESS
PB1.O: LEDB_BRIGHTNESS
PB2.O: LCD_RW
PB3.O: LCD_RS
PB4.?: REELBOX_RESET    -> PWRCTRL-RESET
PB5: MOSI               -> PWRCTRL-SDA/MOSI
PB6: MISO               -> PWRCTRL-SCL/SCK
PB7: SCK/STANDBY-LED2

PC0.I: COL1
PC1.I: COL2
PC2.I: COL3
PC3.I: COL4
PC4.O: LED_STANDBY
PC5.O: LED_INFRARED
PC6.O: LED_RECORD
PC7.O: LCD_RESET

PD0.I: SERIAL_IN (RXD)
PD1.O: SERIAL_OUT (TXD)
PD2.I: IR_RXD (INT0)
PD3.I: VCC5V
PD4.O: ROW1
PD5.O: ROW2
PD6.O: ROW3
PD7.O: REELBOX_WAKEUP   -> PWRCTRL-WAKEIN/MISO

PE0.O: LCD_CS1
PE1.O: LCD_WR
PE2.O: LCD_BRIGHTNESS (OC1B)
*/


int main(void)
{
    int n;
    DDRC = 0xf0;

    while (1)
    {
        PORTC |= 0x10;
        for (n = 0; n < 10; n++)
            _delay_loop_2(60000);
        PORTC &= ~0x10;
        for (n = 0; n < 10; n++)
            _delay_loop_2(60000);
    }
}
