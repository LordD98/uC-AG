/*
 */

// 1ms = 1843;
// 2ms = 3686;

#include <avr/io.h>


int main(void)
{
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
		if ((~PIND) & 1<<PD2)
		{
			OCR1A = 1843;
		}
		else if((~PIND) & 1<<PD3)
		{
			OCR1A = 3686;
		}
	}

    return 0;
}
