/*
 * input capture.cpp
 *
 * Created: 22-Jul-18 4:10:51 PM
 * Author : Aditi
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <A_LCD.h >

int main(void)
{
    DDRD &= ~(1<<PORTD6);
	DDRB |= 0xFF;
	//TCCR1A = 0;
	TCCR1B |= (1<<CS12)|(1<<ICES1);
	TCNT1=0;
	LCDIT();
	
	LCD("aditi");
		
	
    /* Replace with your application code */
    while (1) 
    {
		while(((TIFR) & (1<<ICF1))==0)
		{
			PORTB &= ~(1<<PORTB5);
			_delay_ms(1000);
			
		}
		unsigned int d = ICR1;
		LCDd(d);
		PORTB |= (1<<PORTB5);
		_delay_ms(1000);
		TIFR |= (1<<ICF1);
    }
}

