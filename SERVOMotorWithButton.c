/*
 * lab2.c SERVO motor rotates based on button presses
 *
 * Created: 9/10/18 1:11:56 PM
 * Author : Allie
 */

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB = (1<<PB4);
	DDRA = 0x00;
  TCCR0A = (1<<WGM00) | (1<<COM0B0) | (1<<COM0B1) ;
	TCCR0B = (1<<WGM02) | (1<<CS01) | (1<<CS00);	//Clock division;
	OCR0A = 156;			//PEAK

    while (1)
    {
		_delay_ms(10);
		unsigned char button_pressed = ~PINA;
		if (button_pressed == 0x01)		//180 degrees button 0
			{
				OCR0B = 136;	//Width
			}
		else if (button_pressed == 0x02)	//157.5 degrees button 1
			{
				OCR0B = 138;
			}
		else if (button_pressed == 0x04)	//135 degrees button 2
			{
				OCR0B = 140;
			}
		else if (button_pressed == 0x08)	//112.5 degrees button 3
			{
				OCR0B = 142;
			}
		else if (button_pressed == 0x10)	//67.5 degrees button 4
			{
				OCR0B = 146;
			}
		else if (button_pressed == 0x20)	//45 degrees button 5
			{
				OCR0B = 148;
			}
		else if (button_pressed == 0x40)	//22.5 degrees button 6
			{
				OCR0B = 150;
			}
		else if (button_pressed == 0x80)	//0 degrees button 7
			{
				OCR0B = 152;
			}
		else
			{
				OCR0B = 144;
			}
	}
}
