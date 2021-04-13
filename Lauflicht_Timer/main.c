/*
 * Lauflicht_Timer.c
 *
 * Created: 30.11.2017 19:05:27
 * Author : s-del
 */ 
#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define DDR_LED DDRC
#define PORT_LED PORTC

#define PORT_BTN PORTA
#define PIN_BTN PINA
#define DDR_BTN DDRA
#define BTN0 PA0
#define BTN1 PA1

uint8_t state = 0;

int main(void)
{
	//INit PORT
	DDR_LED = 0xFF;
	PORT_LED = 0xFF;
	DDR_BTN = 0;
	PORT_BTN = 0x03;
	
	//inti timer1
	TCNT1 = 0;
	TCCR1A = 0;
	TCCR1B = (1<<CS11) | (1<<CS10) | (1<<WGM12);
	TIMSK = (1<<OCIE1A);
	OCR1A = 46080;
	sei();
	
    while (1) 
    {
		//if (PIN_BTN && 0x03 == 3)
		//{
		//	continue;
		//}
		if ((PIN_BTN && (1<<BTN1)))
		{
			//schneller
			OCR1A -= 1000;
		}
		else if ((PIN_BTN && (1<<BTN0)))
		{
			//langsamer
			OCR1A += 1000;
		}
		_delay_ms(100);
		while ((~(PIN_BTN && 0x03)) > 0);					
    }
}

ISR(TIMER1_COMPA_vect)
{
	state = (state+1) % 8;
	PORT_LED = ~(1<<state);
}
