/*
 * i2c slave.cpp
 *
 * Created: 16-Jan-19 1:35:33 AM
 * Author : Aditi
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <B_LCD.h>
#include <avr/interrupt.h>
#include <string.h>

void I2C_Slaveassign(char S_address)
{
	TWAR = S_address;
	TWCR = (1<<TWEN)|(1<<TWEA);
}

uint8_t I2C_Slavelisten()
{
	while(1)
	{
		
	
	char status;
	while ((TWCR & (1<<TWINT))==0)
	{
		
		
	}
	status = TWSR & 0xF8;
	if(status == 0x60 || status == 0x68)
	return 0;
	if(status == 0xA8 || status == 0xB0)
	return 1;
	if(status == 0x70 || status == 0x78)
	return 2;
	else
	continue;
	}
}

uint8_t I2C_SlaveTransmit(char data)
{
	char status;
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	status = TWSR & 0xF8;
	if(status == 0xA0)
	{
		TWCR |= (1<<TWINT);
		return -1;
		
	
	}
	if(status == 0xB8)
	return 0;
	if(status == 0xC0)
	{
		TWCR |= (1<<TWINT);
		return -2;
		LCD("NACK Received");
	}
	if(status==0xC8)
	return -3;
	else
	return -4;
}

char I2C_SlaveReceive()
{
	char status;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	PORTB|=(1<<1);
	_delay_ms(100);
	status = TWSR & 0xF8;
	if(status == 0x80 || status == 0x90)
	return TWDR;
	
	if(status == 0x88 || status == 0x98)
	return TWDR;
	
	if(status == 0xA0)
	{
		
		TWCR |= (1<<TWINT);
		return -1;
	}
	else
	return -2;
}


int main(void)
{
	DDRB |= (1<<5)|(1<<6)|(1<<0)|(1<<1);
	
	char name[10];
	int8_t count;
	LCDIT();
	I2C_Slaveassign(0xB0);
	//LCD("Slave");
	int i=0;
	//LCDd(count);
	volatile uint8_t ACK_Status;
    /* Replace with your application code */
    while (1) 
    {
		
		//LCDd(I2C_Slavelisten());
		_delay_ms(100);
		LCDcomm(0x01);
		LCDcomm(0x02);
		switch (I2C_Slavelisten())
		{
			
			
			case 0:
		 {
			// LCD("Receiving");
			 
			 
			 do 
			 {
				 count = I2C_SlaveReceive();
				 
				  
				 if(count == -1 || count == -2)
				 {
					 PORTB |= (1<<0);
					 _delay_ms(100);
					// LCD((char *)"Stop Slave");
					 //_delay_ms(1000) ; 
				 }
				 else
				 {
					 if(count == 'A');
					 {
						 PORTB |= (1<<5)|(1<<6);
						 _delay_ms(100);
					 }
				 /*name[i] = count;
				 LCDcomm(0x80) ; 	 
			     LCDdata(name[i]);
				 i++;*/
				 }	 
				 //LCDcomm(0xC0) ;
				 //LCDf(count) ;
			 } while ((count!=(int8_t)-1));
			// LCD("count") ; 
			 count = 0;
			break;
		 }		 
		 
		 
		    /*case 1:
			{
				
				LCD("Sending");
				do 
				{
					for(i=0 ; i<6; i++)
					{
						
					
					ACK_Status = I2C_SlaveTransmit(name[i]);
					LCDcomm(0xC0);
					LCDdata(name[i]);
					
					}
				} while (ACK_Status == 0);
				LCD("Done");
				break;
			}*/
			default:
			break;
		 }
		 LCDcomm(0x01);
		 LCDcomm(0x02);
		
		 I2C_Slaveassign(0xB0);
	   }
	
}

