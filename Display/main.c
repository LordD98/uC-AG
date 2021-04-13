
#ifndef F_CPU
#define F_CPU  14745600UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "lcd-routines.h"
#include <util/delay.h>

//ADC initialisierung
void adc_init();

//ADC value zurückgeben
uint16_t adc_get_value();

int main(void)
{	
    lcd_init();
	char buff[16];
	adc_init();
	while(1)
	{
		lcd_clear();
		lcd_home();
		lcd_string("Adc-Wert");
		sprintf(buff, "%u", adc_get_value());
		lcd_setcursor(0,1);
		lcd_string(buff);
		_delay_ms(200);
	}
}

void adc_init()
{
    ADMUX |= (1<<REFS0); //Select Ref
    ADMUX |= (1<<MUX0) | (1<<MUX1) | (1<<MUX2); //select 128 Prescaler
    ADCSRA |= (1<<ADPS1) | (1<<ADPS0) | (1<<ADPS2) | (1<<ADEN); //Select input and enable adc
}

uint16_t adc_get_value()
{
	ADCSRA |= (1<<ADSC); //set ADSC to 1 to start conversion
	while(ADCSRA&(1<<ADSC)); //loop until ADSC bit is zero again
	return ADC;
}
