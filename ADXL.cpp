/*
 * ADXL.cpp
 *
 * Created: 19-Dec-18 2:52:10 AM
 * Author : Aditi
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <A_LCD.h>
#include <avr/interrupt.h>

float a;
char b;
void ADCC()
{
	while(!(ADCSRA & (1<<ADIF)))
	{
		
	}
	a = ADC;
	
	switch (b)
	{
		
	
	case 0:
	{
		LCD("X:");
		LCDf(a);
		

		_delay_ms(1000);
		ADCSRA |= (1<<ADIF);
	}
	break;
	
	case 1:
	{
		LCD("Y:");
		LCDf(a);

		_delay_ms(1000);
		ADCSRA |= (1<<ADIF);
	}
	break;
	
	case 2:
	{
		LCD("Z:");
		LCDf(a);
	
		_delay_ms(1000);
		ADCSRA |= (1<<ADIF);
	}
	break;
}
}

void ADC_MUX(unsigned char select)
{
	b = select;
	ADMUX = 0;
	ADMUX |= (1<<REFS0);
	ADMUX |= b&0x07;
	//sei();
	ADC = 0;
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
	ADCSRA |=(1<<ADSC);
	ADCC();
	
	
	
	
}


int main(void)
{
	LCDIT();
	
	/* Replace with your application code */
	while (1)
	{
		LCDcomm(0x01);
		LCDcomm(0x02);
		ADC_MUX(0);
		LCDcomm(0x01);
		LCDcomm(0x02);
		ADC_MUX(1);
		LCDcomm(0x01);
		LCDcomm(0x02);
		ADC_MUX(2);
	}
}

