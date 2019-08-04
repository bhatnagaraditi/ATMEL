/*
 * SPI.cpp
 *
 * Created: 04-Jan-19 10:24:13 PM
 * Author : Aditi
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <B_LCD.h>
#include <avr/interrupt.h>


char a, b;
void SPI_init(char value)
{
	
	char c = value;
	PORTB &= ~(1<<PORTB4);
	SPDR = c;
	//_delay_ms(1000);
	while((SPSR & (1<<SPIF))==0)
	{
		
	}
	
	a = SPDR;
	//_delay_ms(1000);
	/*LCDdata(c);
	LCDcomm(0xC0);
	LCDdata(a);
	LCDcomm(0x01);
	LCDcomm(0x02);*/
}


int main(void)
{
	DDRB |= (1<<DDB4) | (1<<DDB5) | (1<<DDB7);
	DDRB &=  ~(1<<DDB6); 
	PORTB |= (1<<PORTB4);
	LCDIT();
	SPCR|= (1<<SPE)|(1<<MSTR);
    /* Replace with your application code */
    while (1) 
    {
		for(char i=65;i<91;i++)
		{
			SPI_init(i);
			_delay_ms(200);
        }
		
	}
}

