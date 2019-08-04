/*
 * GccApplication2.cpp
 *
 * Created: 13-Jun-18 6:00:51 PM
 * Author : Aditi
 */ 

#include <avr/io.h>
#include <util/delay.h>

void LCDcomm (unsigned char cmmd)
{
	PORTB &= ~(1<<PORTB0);
	
	PORTB |= (1<<PORTB2);
	//unsigned char cmmd1 = cmmd<<4;
	//cmmd1 = (cmmd1 & 0xF0);
	//cmmd = (cmmd & 0xF0);
	PORTA = cmmd;
	_delay_ms(1000);
	PORTB &= ~(1<<PORTB2);
	_delay_ms(20);
	
	
	/*PORTB |= (1<<PORTB2);
	PORTA = cmmd1;
	_delay_ms(20);
	PORTB &= ~(1<<PORTB2);
    _delay_ms(20);
	
	
	
	
	PORTB |= (1<<PORTB3);
	_delay_ms(2);*/
}

void LCDdata (unsigned char data)
{
   PORTB |= (1<<PORTB0);
   PORTB &= ~(1<<PORTB1);
   PORTB |= (1<<PORTB2);
   //unsigned char data1 = (data<<4);
   //data1 = (data1 & 0xF0);
   //data = (data & 0xF0);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           ;
   PORTA = data;
   _delay_ms(1000);
   PORTB &= ~(1<<PORTB2);
   _delay_ms(1000);
  
  
   /*PORTB |= (1<<PORTB2);
   
   PORTA = data1;
   _delay_ms(1000);
   PORTB &= ~(1<<PORTB2);
   _delay_ms(1000);
  
   
   
   PORTB |= (1<<PORTB4);*/
}


int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	
	LCDcomm(0x38);
	LCDcomm(0x0E);
	LCDdata(0x65);
	
	
    /* Replace with your application code */
    while (1) 
    {
		
		//LCDcomm(0x30);
		
		
		
    }
}

