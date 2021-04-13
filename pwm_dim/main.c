/*
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define STEP 150
int8_t step = STEP;
int8_t direction;

//Timer1 Compare Interrupt
ISR( TIMER1_COMPA_vect )
{
    //Bei jedem Interrupt den Dutycycle anpassen und die LEDs toggeln
	PORTC ^= 0xFF;
	if (OCR1A < STEP || OCR1A > ICR1-STEP)
		step *= -1;
	OCR1A += step;
	
	/*
	if (direction > 0)
	{
		if (OCR1A >= ICR1-STEP)
		{
			OCR1A -= STEP;
			direction = -1;
		}
		else
		{
			OCR1A += STEP;
		}
	}
	else
	{
		if (OCR1A <= STEP)
		{
			OCR1A += STEP;
			direction = 1;
		}
		else
		{
			OCR1A -= STEP;
		}	
	}
	*/
}

int main(void)
{
    //PORTC als Ouput mit LEDs am Anfang aus
	DDRC |= 0xFF;
    //Timer auf 0 setzen
	TCNT1 = 0;
    //Duty Cycle initial auf 50% setzen
	OCR1A = 0x7FFF;
    //PMW Phase Correct mit TOP ICR1
	ICR1 = 0xFFFF;
    //TCCR1A/B initialisieren
	//TCCR1A |= 1<<WGM11;
	TCCR1A = 0;
	TCCR1B |= (1<<WGM13) | (1<<CS10);

    //Capture interrupt aktivieren
	TIMSK |= (1<<OCIE1A);
    //Global Interrupts anschalten
	sei();
    while(1);

    return 0;
}
