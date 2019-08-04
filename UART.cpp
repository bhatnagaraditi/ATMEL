/*
 * uart.cpp
 *
 * Created: 25-Aug-18 2:41:31 PM
 * Author : Aditi
 * FINAL CODE DO NOT EDIT AT ANY COST!! 
 */ 
#define F_CPU 16000000UL
#define baudrate (((F_CPU/ (16*USART_BAUDRATE)))-1)
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <A_LCD.h>
//#include <avr/interrupt.h>



void uart(unsigned long USART_BAUDRATE)
{
	UCSRB |= (1<<RXEN)|(1<<TXEN);
	UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UBRRH = (baudrate >> 8);
	UBRRL = baudrate;
	//sei();
	//UCSRB |= (1<<RXCIE);
//	UCSRA|=(1<<RXC);
	
}
void trans(char data)
{
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
}

void uart_sendstring(char *array)
{
	while(*array)
	{
	//	LCDdata(*array);
		trans(*array++);
		
	}
}


void uart_num(int a)
{
	
	char str1[20];
	int i=0;
	int b;
	if(a==0)
	{
		char str2[2];
	    str1[i]='0';
		i++;
		str1[i]='\0';
		strrev(str1);
		uart_sendstring(str1);
	}
	
	if(a>0)
	{
		while(a!=0)
		{
		 b = a%10;
		a = a/10;
		str1[i]='0'+b;
		i++;
		}
		str1[i]='\0';
		strrev(str1);
		uart_sendstring(str1);
	}
	
	if(a<0)
	{
		trans('-');
		a=-a;
		while(a!=0)
		{
			b = a%10;
			a = a/10;
			str1[i]='0'+b;
			i++;
		}
		str1[i]='\0';
		strrev(str1);
		uart_sendstring(str1);
	}
}

unsigned char receive()
{
	while((UCSRA & (1<<RXC))==0)
	{
		
	}
	return (UDR);
} 
 void uart_str(char *string)
 {
	
	char i=0; char p =0; 
	 do
	 
	 {
		 *(string+i)=receive();
		 //LCDdata(*(string+i));
		    p = *(string+i);
		 i++;
	 }
	  
	 while((p !='\r') && (p != '\n'));
	 //while((*(string+i)!=0));
	 i++;
	 *(string+i) = '\0';
	
 }
 
int main()
{
	LCDIT();
 	uart(38400);
	char str3[50];
	//int i=0,j=0;
	 
    /* Replace with your application code */
    while (1) 
    {
	
 
     		
      
/*	uart_sendstring("aditi\r\n");
		
		uart_num(0);
		uart_sendstring("\r\n");
		uart_num(12);
		uart_sendstring("\r\n");
		uart_num(-12);
		uart_sendstring("\r\n");
	    
       	  
		_delay_ms(1000);
	*/
	
	  memset(str3,0,50) ;
	  uart_str(str3);
	  uart_sendstring(str3) ;
	
	 LCD(str3);
	
	//_delay_ms(1000);	
	
	}
}

