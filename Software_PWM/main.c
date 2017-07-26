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
uint16_t read_adc(void);

int main(void)
{
    /* Replace with your application code */
	volatile uint8_t duty = 10, count = 0;
	DDRC = 0xFF;
	PORTC = 0xFF;
	setupADC();
	
    while (1) 
    {
	    duty = read_adc()>>8;
		do
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
		}
		while (count <= 255);
    }
}

void setupADC()
{
	//OLD:
	//ADMUX |= (1<<REFS0)|(7<<MUX0);
	//SFIOR = 0; //free running mode
	//ADCSRA |= 1<<ADPS0 | 1<<ADPS1 | 1<<ADPS2; 
	//ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE);
	
	//NEW
	ADMUX = (1<<REFS0)|(7<<MUX0)|(1<<ADLAR);
	ADCSRA = (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
}

uint16_t read_adc() {
	//ADCR Conversation starten
	ADCSRA |= (1<< ADSC);
	while (ADCSRA&(1<<ADSC));
	return ADC;
}