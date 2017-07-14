/*
 * Software_PWM_wth_ADC.c
 *
 * Created: 12.07.2017 18:39:29
 * Author : s-del
 */ 

#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>

void setupADC(void);

int main(void)
{
    /* Replace with your application code */
	volatile uint8_t duty = 10, count = 0;
	DDRC = 0xFF;
	PORTC = 0xFF;
	setupADC();
	
    while (1) 
    {
		if (count == duty)
		{
			PORTC = 0xFF;
		}
		else if(count == 0)
		{
			PORTC = 0x00;
		}
		count++;
		duty = ADCL;			//works in simulation
    }
}

void setupADC()
{
	ADMUX |= (1<<REFS0)|(7<<MUX0);
	SFIOR = 0; //free running mode
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE);
}

