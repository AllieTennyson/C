/*
 * lab7.c
 * Lab 7 - Read/Write I2C
 *
 * Created 11/8/2018 4:51:48 PM
 * Author : Allie
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <util/twi.h>

#define processorClock 1000000
#define BAUD 9600

volatile uint8_t I2C_address,I2C_data,I2C_pointer = 0;

unsigned char UART_Receive(void)
{
  while ((UCSR0A & (1<<RXC0)) == 0)
  {;}
  return UDR0;
}

void SendSerialMsg(unsigned char status[])
{
  uint16_t i = 0;
  
  while (status[i] != 0)          //Waits for buffer to be empty; terminates with null character
  {
    while((UCSR0A & (1 << UDRE0)) == 0)
    {;}
    
    UDR0 = status[i];          //Prints message character by character, until newline is reached
    i++;
  }
}

void GetSerialMsg(unsigned char message[], uint16_t messagelength)
{
  uint16_t i = 0;
  
  do        //Waits for buffer to be empty; terminates with newline character
  {
    message[i] = UART_Receive();
    
    i++;
    
  } while ((message[i - 1] != 13) && (i < (messagelength-1)));
  
  message[i] = 0;
}

void ADC_init(void)
{
  ADMUX = (1 << REFS0);                                  //selects the voltage (5 volts)
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);    //enables ADC, sets the prescaler (64)
  
}

void uart_init(void)
{
  UCSR0A = (1<<U2X0);                        //doubles transmission speed | (0<<TXC0) no new data in buffer | (0<<RXC0) sets flag when there is no new data
  UCSR0C = (1<<UCSZ11) | (1<<UCSZ10);                //sets character size to 8 bits
  UBRR0L = 12;
  UBRR0H = 0;
  UCSR0B = (1<<RXEN0) | (1<<TXEN0);                //enabled to receive and transmit
}

void I2C_init()
{
  PORTC = (1 << PC1) | (1 << PC0);
  TWCR = (1 << TWEN);
}

void I2CWrite(uint8_t data)
{
  //Start
  if (data == 0x08)
  {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA); //send start bit
    while ((TWCR & (1<<TWINT)) == 0)
    {;}
    return 0;
  }
  
  //Send Address
  if (data == 0x18)
  {
    {
      TWDR = (I2C_address << 1);
      TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA);
      while ((TWCR & (1<<TWINT)) == 0)
      {;}
      return 1;
    }
    
    //Check
    if ((TWSR & 0xF8) != 0x18)
    {
      return 1;
    }
    
    return 0;
  }
  
  //Sending Pointer and Data
  
  if(data == 0x28)
  {
    //Send pointer
    TWDR = I2C_pointer;
    TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWIE);
    while ((TWCR & (1<<TWINT)) == 0){;}
    
    //Send Data
    TWDR = I2C_data;
    while ((TWCR & (1<<TWINT)) == 0){;}
    
    //Check
    if ((TWSR & 0xF8) != 0x28)
    {
      return 1;
    }
    
    //Stop
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1<<TWSTO); //send stop bit
    return 0;
  }
}

void I2CReadByte(uint8_t data)
{
  //Start
  if (data == 0x08)
  {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA); //send start bit
    while ((TWCR & (1<<TWINT)) == 0){;} // Wait for Flag = 1
    return 0;
  }
  //Send Address
  if (data == 0x18)
  {
    TWDR = (I2C_address << 1);
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA);
    while ((TWCR & (1<<TWINT)) == 0){;}
    
    TWDR = I2C_pointer;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1<<TWINT)) == 0){;}
    
    TWDR = (I2C_address << 1) | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1<<TWINT)) == 0){;}
    return 0;
  }
  //Repeated start
  if (data == 0x10)
  {
    TWDR = (I2C_address << 1) | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA);
    while ((TWCR & (1<<TWINT)) == 0){;}
    return 0;
  }
  //Send Data 2
  if (data == 0x40) {
    TWDR = (I2C_address << 1);
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA);
    while ((TWCR & (1<<TWINT)) == 0){;}
    
    TWDR = I2C_pointer;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1<<TWINT)) == 0){;}
    
    TWDR = (I2C_address << 1) | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1<<TWINT)) == 0){;}
    return 0;
  }
  //Receive data
  if (data == 0x50)
  {
    TWCR = (1<<TWINT) | (1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0){;}
    I2C_data = TWDR;
    
    //Stop
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1<<TWSTO);
  }
}

int main(void)
{
  uart_init();
  I2C_init();
  
  I2C_address = 0x48;
  I2C_data = 0x20;
  I2C_pointer = 0x01;
  
  
  I2CWrite(0x08); //start
  I2CWrite(0x18); //send address
  I2CWrite(0x28); //send data and stop
  
  SendSerialMsg("Read config register \n");
  I2CWrite(0x08); //start
  I2CWrite(0x18); //send address
  I2CWrite(0x50); //send data and stop
  
  unsigned char result[64];
  sprintf(result,"Data: %02X \n", I2C_data);
  SendSerialMsg(result);
  //SendSerialMsg(I2C_data);
  
  
  
  
  while (1)
  {
    // I2CWrite
    
    /*
     I2CWrite(0x08); // Start
     I2CWrite(0x18); // Send address
     I2CWrite(0x28); // Send data and stop
     
     I2CWrite(0x08); // Start
     I2CWrite(0x18); // Send address
     I2CWrite(0x50); // Send data and stop
     _delay_ms(50);
     _delay_ms(50);
     */
    
    /*
     I2CReadByte(0x08);    // Start
     I2CReadByte(0x18);    // Read address and pointer
     I2CReadByte(0x50);    // Read data and stop
     I2CReadByte(0x10);    // Repeated Start
     I2CReadByte(0x40);
     I2CReadByte(0x50);
     _delay_ms(50);
     _delay_ms(50);
     */
    
    
    //I2CRead8Byte(0x08);
    //I2CRead8Byte(0x08);
  }
}
