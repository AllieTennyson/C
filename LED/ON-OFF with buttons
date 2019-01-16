/*
 * lab3.c
 *
 * Created: 9/10/18 1:11:56 PM
 * Author : Allie
 *
 * On the falling edge (button pressed) the LED turns on
 * On the rising edge (button not-pressed) the LED turns off
 */

#include <avr/io.h>
#include <avr/interrupt.h>


ISR(INT0_vect)
	{
		if (PIND != 0xFF)
			PORTB = 0xFE;
		else
			PORTB = ~0x00;
	}

int main(void)
{
	DDRB = 0xFF;
	DDRD = 0x00;
	PORTB = 0xFF;


	EICRA = (1 << ISC00);	//Checks falling/rising edge
	EIFR = (1 << INTF0);	//Clears INT0 flag bit
	EIMSK = (1 << INT0);	//Enables global interrupts
	sei();

	while(1)
	{

	}
}
