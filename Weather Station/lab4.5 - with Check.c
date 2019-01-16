/*
 * lab4.c
 * Lab 4.5 - Use Weather Machine
 *
 * Created: 10/18/18 2:15:49 PM
 * Author : Allie
 */ 


#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

volatile unsigned char character_counter = 0;
volatile int edge_count = 0;
volatile unsigned char check = 0;

ISR(PCINT3_vect)
{
	edge_count += 1;
	check = 1;
}

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
	PORTD = (1<<PD6);
	uart_init();
	PCIFR = (1<<PCIF3);
	PCICR = (1<<PCIE3);												//enables interrupts
	PCMSK3 = (1<<PCINT30);											//interrupt on PC3
	sei();
	unsigned char windSpeed[256];
	uint16_t wind_speed = 0.0;

	
	while(1)
	{
		unsigned char stringMessage[64];
		unsigned char option;
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
				SendSerialMsg("Wind Speed: \n");
				while(1)
				{
					{
						edge_count = 0;
						_delay_ms(250);
						_delay_ms(250);
						_delay_ms(250);
						_delay_ms(250);
						wind_speed = ((edge_count * 1.429)/2);
						sprintf(windSpeed, "%d \n", wind_speed);
						SendSerialMsg(windSpeed);
					}
				}
				

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


