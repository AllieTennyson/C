/*
 * lab2.c
 *
 * Created: 9/6/18 2:36:22 PM
 * Author : Allie
 */

#include <avr/io.h>
#include <util/delay.h>


/* Creates chasing LEDs that can be controlled by switches
*  Switch 0 starts/stops LEDs
*  Switch 1 changes direction so LEDs chase to the left
*  Switch 2 changes direction so LEDs chase to the right

*  Case 0
*     - means state =  0
*     - shifts LED to the left until it equals 0 then it sets the LED to 1
*  Case 1
*     - means state = 1
*     - shifts LED to the right. If it equals zero then it sets the LED to be 0x80
*/
int main(void)
{
	DDRB = 0xFF;
	DDRA = 0x00;
	unsigned char state = 0;
	unsigned char moving = 0;
	unsigned char LED = 1;
	unsigned char button_pressed = 0;

	while (1)
	{
		_delay_ms(60);
		unsigned char not_a = ~PINA;
		PORTB = ~LED;

		if (moving)
		{
			switch(state)
			{
				case 0:
					LED = LED << 1;
					if (LED == 0)
						LED = 1;
					break;

				case 1:
					LED = LED >> 1;
					if (LED == 0)
						LED = 0x80;
					break;

				default:
					break;
			}
		}

		if (button_pressed == 0)
		{
			if ((not_a & 1) == 1)					//Button 0 was pressed. Either stops/starts the lights moving
			{
				button_pressed = 1;
				moving = ~moving;
			}

			else if ((not_a & 2) == 2)		//Button 1 was pressed. Lights start chasing to the left
			{
				button_pressed = 1;
				state = 0;
			}

			else if ((not_a & 4) == 4)		//Button 2 was pressed. Lights start chasing to the right
			{
				button_pressed = 1;
				state = 1;
			}
		}

		if (not_a == 0)
			button_pressed = 0;
	}
}
