/*
 * lab4.c
 * Lab 4.4 - Uses interrupts - from the ISR
 *
 * Created: 10/8/18 1:05:49 PM
 * Author : Allie
 */


#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char character_counter = 0;
volatile unsigned char check = 0;

ISR(USART0_RX_vect)
{
	character_counter += 1;
	check = 1;
}

void uart_init(void)
{
	UCSR0A = (1<<U2X0);												//doubles transmission speed | (0<<TXC0) no new data in buffer | (0<<RXC0) sets flag when there is no new data
	UCSR0C = (1<<UCSZ11) | (1<<UCSZ10);								//sets character size to 8 bits
	UBRR0L = 12;
	UBRR0H = 0;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);								//enabled to receive and transmit
	PCICR = (1<<PCIE0);												//enables interrupts
}

unsigned char UART_Receive(void)
{
	while ((UCSR0A & (1<<RXC0)) == 0)
	{;}
	return UDR0;
}

void SendSerialMsg(unsigned char status[])
{
	uint16_t i = 0;

	while (status[i] != 0)					//Waits for buffer to be empty; terminates with null character
	{
		while((UCSR0A & (1 << UDRE0)) == 0)
		{;}

		UDR0 = status[i];					//Prints message character by character, until newline is reached
		i++;
	}
}

void GetSerialMsg(unsigned char message[], uint16_t messagelength)
{
	uint16_t i = 0;

	do				//Waits for buffer to be empty; terminates with newline character
	{
		message[i] = UART_Receive();

		i++;

	} while ((message[i - 1] != 13) && (i < (messagelength-1)));

	message[i] = 0;
}


int main(void)
{
	EIMSK = (1<<INT0);
	uart_init();
	UCSR0B |= (1 << RXCIE0);
	sei();
	unsigned char message[64];

	while(1)
	{
		if (check == 1)
		{
			GetSerialMsg(message, 64);
			SendSerialMsg(message);
			check = 0;
		}
	}
}
