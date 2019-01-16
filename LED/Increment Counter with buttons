/*
 * lab2.c
 *
 * Created: 9/6/18 2:36:22 PM
 * Author : Allie
 */

#include <avr/io.h>
#include <util/delay.h>


/* LEDs increment according to switch instructions
*  Switch 0 increments the counter
*  Switch 1 decrements the counter
*  Switches 0 & 1 together clear the counter
*/
int main(void)
{
	unsigned char i = 0;
	DDRB = 0xFF;
	DDRA = 0x00;
	unsigned char not_a;

	while (1)
	{
		_delay_ms(60);
		not_a = ~PINA;

		if ((not_a & 3) == 3)	//switch 0 and 1 together clear the count
		{
			i = 0;
			PORTB = ~i;
		}

		else if ((not_a & 1) == 1)	//switch 0 increments count
		{
			i++;
			PORTB = ~i;
		}

		else if ((not_a & 2) == 2)	//switch 1 decrements count
		{
			i--;
			PORTB = ~i;
		}

		else
		{
			PORTB = 0xFF;
		}
	}
}
