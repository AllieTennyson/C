/*
 * lab2.c
 *
 * Created: 9/6/18 2:36:22 PM
 * Author : Allie
 */

#include <avr/io.h>
#include <util/delay.h>


// Program creates LED binary counter based off of built in clock

int main(void)
{
	DDRB = 0xFF;
	unsigned char i = 0;

	while (1)
	{
		_delay_ms(80);
		PORTB = ~i;
		i++;
	}
}
