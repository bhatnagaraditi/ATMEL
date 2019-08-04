/*
 * GPS.cpp
 *
 * Created: 07-Oct-18 6:47:30 PM
 * Author : Aditi
 */ 
#define F_CPU 16000000UL
#define baudrate (((F_CPU/ (16*USART_BAUDRATE)))-1)
#define SREG _SFR_IO8(0x3f)
#include <avr/io.h>
#include <A_LCD.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>

void convert_time_to_UTC();
void convert_to_degrees(char *);

char Latitude[15], Longitude[15], Time[15], Altitude[8], satellite[3];
char degrees[20];                           /* save latitude or longitude in degree */
char GGA_string[150];                       /* save GGA string */
uint8_t pointer_GGA[20];                    /* to store instances of ',' */
char GGA_VALUE[3];
char lat_dir,long_dir;
uint8_t a ;
volatile uint16_t index, comma;

bool	GGASTRING = false;


void uart(unsigned long USART_BAUDRATE)
{
	UCSRB |= (1<<RXEN)|(1<<TXEN);
	UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UBRRH = (baudrate >> 8);
	UBRRL = baudrate;
	sei();
	UCSRB |= (1<<RXCIE);
	
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

void GPS_time(){
	cli();
	uint8_t time_index=0;

	/* parse Time in GGA string stored in buffer */
	for(uint8_t i = 0; GGA_string[i]!=',';  i++){
		
		Time[time_index] = GGA_string[i];
		time_index++;
	}
	convert_time_to_UTC();
	sei();
}


void convert_time_to_UTC()
{
	unsigned int hour, min, sec;
	uint32_t Time_value;
	
	Time_value = atol(Time);       /* convert string to integer */
	hour = (Time_value / 10000);          /* extract hour from integer */
	min  = (Time_value % 10000) / 100;    /* extract minute from integer */
	sec  = (Time_value % 10000) % 100;    /* extract second from integer*/
	
	 hour = hour+5;
	 min = min+30;
     if(min>=30)
	 {
		 hour = hour + (min/60);
		 min = min%60;
		 
	 }
	 
	 if(hour>=18)
	 {
		 hour = hour%24;
	 }

	sprintf(Time, "%d:%d:%d\r\n", hour,min,sec);

	
}

void GPS_latitude(uint16_t lat_pointer){
	cli();
	uint8_t lat_index;
	uint8_t i = lat_pointer+1;
	lat_index=0;
	
	/* parse Latitude in GGA string stored in buffer */
	for(;GGA_string[i]!=',';i++){
		Latitude[lat_index]= GGA_string[i];
		lat_index++;
	}
	
	Latitude[lat_index++] = GGA_string[i++];
	Latitude[lat_index]   = GGA_string[i];  /* get direction */
	lat_dir          = Latitude[lat_index];
	convert_to_degrees(Latitude);
	sei();
}

void GPS_longitude(uint16_t long_pointer){
	cli();
	uint8_t long_index;
	uint8_t i = long_pointer+1;
	long_index=0;
	
	/* parse Longitude in GGA string stored in buffer */
	for(; GGA_string[i]!=','; i++){
		Longitude[long_index]= GGA_string[i];
		long_index++;
	}
	
	Longitude[long_index++] = GGA_string[i++];
	Longitude[long_index]   = GGA_string[i]; /* get direction */
	long_dir            = Longitude[long_index];
	convert_to_degrees(Longitude);
	sei();
}

void convert_to_degrees(char *raw){
	
	double value;
	float decimal_value,temp;
	
	int32_t degrees_raw;
	
	float position;
	value = atof(raw);    /* convert string into float for conversion */
	
	/* convert raw latitude/longitude into degree format */
	decimal_value     = (value/100);
	degrees_raw       = (int)(decimal_value);
	temp              = (decimal_value - (int)decimal_value)/0.6;
	position          = (float)degrees_raw + temp;
	
	dtostrf(position, 6, 4, degrees);  /* convert float value into string */
}

void GPS_altitude(uint16_t alt_pointer){
	cli();
	uint8_t alt_index;
	uint8_t i = alt_pointer+1;
	alt_index=0;
	/* parse Altitude in GGA string stored in buffer */
	for(; GGA_string[i]!=','; i++){
		Altitude[alt_index]= GGA_string[i];
		alt_index++;
	}
	//Altitude[alt_index] = GGA_string[++i];
	
	sei();
}

void GPS_sat(uint16_t sat_pointer)
{
	cli();
	uint8_t sat_index;
	uint8_t i = sat_pointer+1;
	sat_index=0;
	/* parse no. of satellites in GGA string stored in buffer */
	for(; GGA_string[i]!=','; i++){
		satellite[sat_index]= GGA_string[i];
		sat_index++;
	}
	
	sei();
}




ISR(USART_RXC_vect)
{
	uint8_t oldsreg = SREG;
	cli();
	char receive = UDR;
	
	if(receive == '$')
	{
		index = 0;
		comma = 0;
		GGASTRING = false;
	}
	
	else if(GGASTRING == true)
	{
		if (receive == ',')
		pointer_GGA[comma++]=index;
		
		GGA_string[index++] = receive;
	}
	
	else if(GGA_VALUE[0] == 'G' && GGA_VALUE[1] == 'G' && GGA_VALUE[2] ==  'A')
	{
		GGASTRING = true;
		GGA_VALUE[0] = 0; GGA_VALUE[1] = 0; GGA_VALUE[2] = 0;
	}
	
	else
	{
		GGA_VALUE[0] = GGA_VALUE[1];  GGA_VALUE[1] = GGA_VALUE[2]; GGA_VALUE[2] = receive;
	}
	
	SREG = oldsreg;
	
}

int main(void)
{
	      
	 
		index=0;
		memset(GGA_string, 0, 150);
		memset(degrees,0,20);
		
		LCDIT();                 /* initialize LCD16x2 */
		_delay_ms(3000);         /* wait for GPS receiver to initialize */
		uart(9600);
		LCD("Welcome to the");
		LCDcomm(0xC0);
		LCD("Third Review");
		_delay_ms(4000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		LCD("Name of the");
		LCDcomm(0xC0);
		LCD("Respective");
		_delay_ms(4000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		LCD("Students are:");
		LCDcomm(0xC0);
		LCD("Aditi Bhatnagar");
		_delay_ms(4000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		LCD("Roll No.:");
		LCDcomm(0xC0);
		LCDd(16);
		LCD("BEC");
		LCDd(0);
		LCDd(0);
		LCDd(8);
		_delay_ms(4000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		LCD("Aayushi Maniar");
		LCDcomm(0xC0);
		LCDd(16);
		LCD("BEC");
		LCDd(0);
		LCDd(0);
		LCDd(2);
		_delay_ms(4000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		LCD("Name of the");
		LCDcomm(0xC0);
		LCD("Project:");
		_delay_ms(4000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		LCD("Safety Watch ");
		LCDdata(0x3A);
		LCDdata(0x29);
		LCDcomm(0xC0);              /* initialize USART with 9600 baud rate */
		sei();
        
		
    /* Replace with your application code */
    while 
	(1) 
    {
		_delay_ms(1000);
		LCD("Time: ");
		//uart_str("Time: ");
		GPS_time();                 /* Extract Time in UTC */
		LCD(Time);
		uart_sendstring("TIME (IST) : ") ; 
		uart_sendstring(Time) ;  
		LCDcomm(0xC0);
		_delay_ms(1000);
		
		LCD("Lat: ");
		GPS_latitude(pointer_GGA[0]);   /* Extract Latitude */
		LCD(degrees); /* display latitude in degree */
		LCDdata(0xDF);
		LCDdata(lat_dir);
		uart_sendstring("Latitude : ");
		uart_sendstring(degrees); /* display latitude in degree */
		uart_sendstring(" deg. N ") ; 
		//trans(lat_dir);
		uart_sendstring("\r\n");
		memset(degrees,0,20);
		_delay_ms(1000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		
		
		LCD("Long: ");
		GPS_longitude(pointer_GGA[2]);  /* Extract Longitude */
		LCD(degrees);                   /* display longitude in degree */
		LCDdata(0xDF);
		LCDdata(long_dir);
		uart_sendstring("Longitude : ");     
		uart_sendstring(degrees); /* display latitude in degree */
		uart_sendstring(" deg. E") ;
		//trans(long_dir);
		uart_sendstring("\r\n");
		memset(degrees,0,20);
		LCDcomm(0xC0);
		_delay_ms(1000);
		 
		 
		LCD("Alt: ");
		GPS_altitude(pointer_GGA[7]);   /* Extract Altitude in meters*/
		LCD(Altitude);
		LCD("Metres");
		uart_sendstring("Altitude : ");
		uart_sendstring(Altitude); /* display latitude in degree */
		trans('M');
		uart_sendstring("\r\n");
		_delay_ms(1000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		
		LCD("Sat: ");
		GPS_sat(pointer_GGA[5]);
		LCD(satellite);
		_delay_ms(1000);
		LCDcomm(0x01);
		LCDcomm(0x02);
		
    }
}

