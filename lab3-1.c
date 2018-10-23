/*
 * lab3.c
 *
 * Created: 9/10/18 1:11:56 PM
 * Author : Allie
 *
 * On the falling edge (button pressed) the LED turns on and the message LED ON is sent to UART
 * On the rising edge (button not-pressed) the LED turns off and the message LED OFF is sent to the UART
 */

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

volatile unsigned char check;

ISR(PCINT0_vect)
	{
		if (PINA != 0xFF)					//if button pressed
		{
			check = 1;
			PORTB = 0xFE;						//LED turns on
		}
		else
		{
			check = 0;
			PORTB = 0xFF;						//LED turns off
		}
	}

void SendSerialMsg(unsigned char message[])
{
	uint16_t i = 0;

	while (message[i] != 0 )				//Waits for buffer to be empty
	{
		while(0 == (UCSR0A & (1 << UDRE0)))
		{;}

		UDR0 = message[i];					//Prints message character by character
		i++;
	}
}

void uart_init(void)
{
	UCSR0A = (1<<U2X0);										//set UBBR to 12
	UCSR0C = (1<<UCSZ11) | (1<<UCSZ10);		//sets character size to 8 bits
	UBRR0L = 12;
	UBRR0H = 0;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);			//enabled to receive and transmit
}

int main(void)
{
	DDRB = 0xFF;
	DDRA = 0x00;
	PORTB = 0xFF;
	unsigned char LED_on[] = "LED ON\n";			//message to send to UART when LED is on
	unsigned char LED_off[] = "LED OFF\n";		//message to send to UART when LED is off
	EICRA = (1 << ISC00);									//Checks falling/rising edge
	PCICR = (1 << PCIE0);									//Enables interrupt
	PCMSK0 = (1 << PCINT0);
	sei();

	while(1)
	{
		if (check == 1)
			SendSerialMsg(LED_on);
		else
			SendSerialMsg(LED_off);
	}
}
