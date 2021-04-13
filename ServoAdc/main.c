/*
 * ServoAdc.c
 *
 * Created: 21.12.2017 19:06:05
 * Author : s-del
 */ 

#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>

#define SERVO_TOP  3686  
#define SERVO_BOTTOM 1843
#define SERVO_DIF 1843

uint16_t read_adc();
void moveServo(uint16_t pos);
void moveServoSlow(uint16_t pos);

int main(void)
{
	ADMUX = (1<<REFS0)|(7<<MUX0);
	ADCSRA = (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
	
	
	//PD5 als output
	DDRD = (1<<PD5);
	PORTD |= (1<<PD2)|(1<<PD3);

	//timer conf
	// Top: 36864-1 = 36863
	TCNT1 = 0;
	ICR1 = 36863;
	OCR1A = 2764;
	TCCR1A |= (1<<COM1A1)|(1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13)|(1<<CS11);
		
    while(1)
	{
		_delay_ms(250);
		moveServoSlow((uint16_t)(((float)read_adc()/1023.0) * SERVO_DIF + SERVO_BOTTOM));
	}
}

uint16_t read_adc()
{
	//ADCR Conversation starten
	ADCSRA |= (1<< ADSC);
	while (ADCSRA&(1<<ADSC));
	return ADC;
}

void moveServoSlow(uint16_t pos)
{
	//#define DURATION 1000; // 1000 ms
	#define STEP 1
	//int32_t difference = pos - OCR1A;
	if (pos > OCR1A)
	{
		while(OCR1A < pos + STEP)
		{
			OCR1A += STEP;
			_delay_ms(1);
		}
		OCR1A = pos;
	}
	else
	{
		while(OCR1A > pos - STEP)
		{
			OCR1A -= STEP;
			_delay_ms(1);
		}
		OCR1A = pos;
	}
}

void moveServo(uint16_t pos)
{
	OCR1A = pos;
}