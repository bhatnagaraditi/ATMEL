/*
 * i2c.cpp
 *
 * Created: 14-Jan-19 11:43:27 PM
 * Author : ADITI
 */ 


#define F_CPU 8000000UL
#define SCL_CLK 100000L	
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))
#include <avr/io.h>
#include <util/delay.h>
#include <B_LCD.h>
#include <avr/interrupt.h>
#include <string.h>

void I2C_Init()			/* I2C initialize function */
{
	TWBR = BITRATE(TWSR=0x00);	/* Get bit rate register value by formula */
}

void I2C_Start(char W_address)
{
	LCDcomm(0x01);
	LCDcomm(0x02);
	char status;
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	status = TWSR & 0x08;
	if(status!=0x08)
	{
		LCD("Error in Start");
	}
	TWDR = W_address;
	//LCD("heya");
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	//LCD("heya1");
	status = TWSR & 0xF8;
	if(status== 0x18)
	{
		LCD("Start done");
	}
	else if(status == 0x20)
	{
		LCD("NACK_S Received");
	}
	else if(status == 0x38)
	{
		LCD("Arbitration_S Lost");
	}
	else
	{
		LCD("Sad Life_S");
	}
	
	
}

void Repeated_Start(char R_address)
{
	LCDcomm(0x01);
	LCDcomm(0x02);
	char status;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	status = TWSR & 0xF8;
	if (status!=0x10)
	{
		LCD("Error aaya baba");
	}
	TWDR = R_address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	LCDcomm(0x01);
	LCDcomm(0x02);
	status = TWSR & 0xF8;
	if(status == 0x40)
	{
		LCD("Repeated_S received");
	}
	else if(status == 0x48)
	{
		LCD("NACK_RS received");
	}
	else if (status == 0x38)
	{
		LCD("Arbitration Lost_RS");
		
	}
	
	else
	{
		LCD("Sad Life Again");
	}
	
}

void I2C_Data(char data)
{
	LCDcomm(0x01);
	LCDcomm(0x02);
	char status;
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	status = TWSR & 0xF8;
	if(status == 0x28)
	{
		LCD("Data transmitted");
	}
	
	else if(status == 0x30)
	{
		
		LCD("Data_NACK");
	}
	
	else
	{
		LCD("dgnjdcm");
	}
}

char Read_ACK()
{
	LCDcomm(0x01);
	LCDcomm(0x02);
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	return TWDR;
}

char Read_NACK()
{
	LCDcomm(0x01);
	LCDcomm(0x02);
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0)
	{
		
	}
	return TWDR;
}

void I2C_Stop()
{
	LCDcomm(0x01);
	LCDcomm(0x02);
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while((TWCR & (1<<TWSTO))==1)
	{
		LCD("fasa hun");
	}
	LCD("stopped");
}



int main(void)
{
	char name[10] = "Aditi";
	LCDIT();
	I2C_Init();
	I2C_Start(0xB0);
	for(int i=0; i<6; i++)
	{
		
		//LCD("Sending_M");
		I2C_Data(name[i]);
	}
	
	//I2C_Stop();
	//LCD("out");
	_delay_ms(30);
	//I2C_Start(0xB0);
	//I2C_Data(0x00);
	Repeated_Start(0xB1);
	for(int i=0; i<5; i++)
	{
		//LCD("Receiving_M");
		//LCDcomm(0xC0);
		//_delay_ms(300);
		LCDdata(Read_ACK());
		_delay_ms(300);
	}
	Read_NACK();
	//_delay_ms(300);
	I2C_Stop(); 
    /* Replace with your application code */
    while (1) 
    {
    }
}

