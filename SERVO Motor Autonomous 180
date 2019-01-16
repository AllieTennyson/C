/*
 * lab2.c Controls SERVO motor to rotate 180 degrees L and R
 *
 * Created: 9/10/18 1:11:56 PM
 * Author : Allie
 */

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRA = 0x00;

	//Phase Correct PWM
	//sets OC0B on Compare Match when up-counting. Clears
	//OC0B on Compare Match when down-counting
	TCCR0A = (1<<WGM00) | (1<<COM0B0) | (1<<COM0B1);

	//Clock division
	//64 from prescaler
	TCCR0B = (1<<WGM02) | (1<<CS01) | (1<<CS00);
	OCR0A = 156;			//PEAK
	DDRB = (1<<PB4);

	//OCR0B = 151;	//"180" degrees
	OCR0B = 137;	//"0" degrees

	while (1)
	{
		if (OCR0B < 150)
		{
			for(int i = 0; i < 15; i++)
			{
				OCR0B += 1;
				_delay_ms(300);
			}
		}

		else if (OCR0B > 150)
		{
			for (int i = 0; i < 15; i++)
			{
				OCR0B -= 1;
				_delay_ms(300);
			}
		}
	}



}
