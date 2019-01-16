/*
 * lab4.c
 * Lab 4.3 - Uses interrupts not 
 *
 * Created: 10/8/18 1:05:49 PM
 * Author : Allie
 */ 


#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUFFER_SIZE	64

volatile unsigned char char_cnt = 0;
volatile unsigned char buffer[BUFFER_SIZE];
volatile unsigned char flag;

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

ISR(USART0_RX_vect)
{	
	buffer[char_cnt] = UDR0;
	
	if((buffer[char_cnt] == 13) || (char_cnt >= (BUFFER_SIZE - 2)))
	{
		flag = 1;
	}
	char_cnt++;
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

int main(void)
{
	EIMSK = (1<<INT0);
	uart_init();
	UCSR0B |= (1 << RXCIE0);
	sei();
	
	while(1)
	{
		if (flag == 1)
		{
			buffer[char_cnt] = 0;
			SendSerialMsg(buffer);
			char_cnt = 0;
			flag = 0;
		}
	}
}
