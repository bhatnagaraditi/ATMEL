/*
 * ADC.cpp
 *
 * Created: 26-Jun-18 4:31:19 PM
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
	/*if(a>1000)
	{
		LCDd(0);
	}
	a = a/100;
	a = a*10;
	a = a-100;
	if (a<0)
	{
		a = -a;
		LCDf(a);
	}
	*/ // my calculation for soil moisture sensor
	
	
	// nice calculation
	
	a = 1023-a;
	a = a/1023;
	a = a*100;
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

/*ISR(ADC_vect)
{
	int a = ADC;
	LCDd(a);
	_delay_us(1000);
	ADCSRA |= (1<<ADSC);
}*/

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
