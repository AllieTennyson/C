/*
 * lab3.c
 * Lab 3.2 - UART LED ON/LED OFF
 *
 * Created: 9/10/18 1:11:56 PM
 * Author : Allie Tennyson
 *
 * On the falling edge (button pressed) the LED turns on
 * On the rising edge (button not-pressed) the LED turns off
 */

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char check;

ISR(PCINT0_vect)
	{
		if (PINA != 0xFF)					//if button pressed
		{
			check = 1;
			PORTB = 0xFE;					//LED turns on
		}
		else
		{
			check = 0;
			PORTB = 0xFF;					//LED turns off
		}
	}

void SendSerialMsg(unsigned char message[])
{
	uint16_t i = 0;

	while (message[i] != 0 )				//Waits for buffer to be empty
	{
		while((UCSR0A & (1 << UDRE0)) == 0)
		{;}

		UDR0 = message[i];					//Prints message character by character
		i++;
	}
}

void uart_init(void)
{
	UCSR0A = (1<<U2X0);						//set UBBR to 12
	UCSR0C = (1<<UCSZ11) | (1<<UCSZ10);		//sets character size to 8 bits
	UBRR0L = 12;
	UBRR0H = 0;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);		//enabled to receive and transmit
}

int main(void)
{
	DDRB = 0xFF;
	DDRA = 0x00;
	PORTB = 0xFF;
	unsigned char LED_on[] = "LED ON\n";
	unsigned char LED_off[] = "LED OFF\n";
	EICRA = (1 << ISC00);					//Checks falling/rising edge
	PCICR = (1 << PCIE0);					//Enables interrupt
	PCMSK0 = (1 << PCINT0);
	uart_init();
	sei();

	while(1)
	{
		_delay_ms(100);
		if (check == 1)
		{
			SendSerialMsg(LED_on);
			check = 2;
		}
		else if (check == 0)
		{
			SendSerialMsg(LED_off);
			check = 2;
		}
	}
}
