/*
 * lm35.cpp
 *
 * Created: 18-Dec-18 3:18:33 PM
 * Author : Aditi
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <A_LCD.h>
#include <avr/interrupt.h>

void ADCC()
{
	while(!(ADCSRA & (1<<ADIF)))
	{
		
	}
	float a = ADC;
	LCDf(a);
	LCDcomm(0xC0);
	a = (a*5)/1023;
	a = a*1000;
	a = a/10;
	
	LCDf(a);
	_delay_ms(4000);
	ADCSRA |= (1<<ADIF);
	
	
}
void ADCV()
{
	ADMUX |= (1<<REFS0);
	//sei();
	ADCSRA |= (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRA |=(1<<ADSC);
	
}

int main(void)
{
	LCDIT();
    /* Replace with your application code */
    while (1) 
    {
		ADCV();
		ADCC();
		LCDcomm(0x01);
		LCDcomm(0x02);
    }
}

