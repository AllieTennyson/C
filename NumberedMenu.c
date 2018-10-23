/*
 * lab3.c
 * Lab 3.3 - UART Displays Numbered menu, user types in number option. Number outside of scope results in an error message
 *
 * Created: 9/10/18 1:11:56 PM
 * Author : Allie Tennyson
 *
 * A user
 */

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void uart_init(void)
{
	UCSR0A = (1<<U2X0);												//doubles transmission speed | (0<<TXC0) no new data in buffer | (0<<RXC0) sets flag when there is no new data
	UCSR0C = (1<<UCSZ11) | (1<<UCSZ10);								//sets character size to 8 bits
	UBRR0L = 12;
	UBRR0H = 0;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);								//enabled to receive and transmit
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
	unsigned char stringMessage[64];
	uart_init();
	unsigned char option;

	while(1)
	{
		//SendSerialMsg(stringMessage);

		SendSerialMsg("Menu: \n");
		SendSerialMsg("1. Temperature \n");
		SendSerialMsg("2. Wind Speed \n");
		SendSerialMsg("3. Wind Direction \n");
		SendSerialMsg("4. SD Card Contents \n \n");

		GetSerialMsg(stringMessage, 64);

		option = atoi(stringMessage);

		switch(option)
		{
			case 1:
				SendSerialMsg("Temperature goes here \n");
			break;

			case 2:
				SendSerialMsg("Wind Speed goes here \n");
			break;

			case 3:
				SendSerialMsg("Wind Direction goes here \n");
			break;

			case 4:
				SendSerialMsg("SD Card Contents goes here \n");
			break;

			default:
				break;
		}
	}
}
