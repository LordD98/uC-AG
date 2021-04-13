/*
 */

#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR( INT0_vect )
{
    //TCNT mit halber Bitzeit laden
	TCNT1 = 383;
    //Timer Compare A Interrupt anschalten
	TIMSK |= 1<<OCIE1A;
    //INT0 Interrupt abschalten
	GICR &= ~(1<<INT0);
}

ISR(TIMER1_COMPA_vect)
{
	static uint8_t bit_count = 0;
	static uint8_t data;

        if( bit_count == 0 )
		{
            //PD2 entspricht dem startbit
            //in rec zustand wechseln
			bit_count = 1;
            //data init
			data = 0;
        }
        else if( bit_count < 10 )
		{
            //Zustand erhoehen
			bit_count++;
            //Bit aus PD2 lesen und in daten speichern
			//data |= ((PIND&(1<<PIND2))>>2)<<(9-bit_count);			// 0b00000100
			data = data >> 1;
			data |= ((PIND&(1<<PIND2))<<5);
        }
        else 
		{
            //zustand auf idle
			bit_count = 0;
            //Timer interrupt aus
			TIMSK &= ~(1<<OCIE1A);
            //INT0 interrupt an
			GICR |= 1<<INT0;
            //flags loeschen
            GIFR = 0xFF;

            //Daten ausgeben
            PORTC = ~data;
        }
}

int main(void)
{
    //PORTC als output
	DDRC = 0xFF;
    //default aus
	PORTC = 0;
	
    //PORTD ans Input
	DDRD = 0;
    //PD2 Pullup an
	PORTD = 0xFF;

    //Timer1
	TCCR1A = 1<<WGM12;
    //CTC
	TCCR1B = (1<<WGM13)|(1<<CS10);
    //soll 9600 mal pro sec compare interrupt
    //ausloesen
	ICR1 = 767;

    //INT0 auf fallende Flanke
    //INT0 anschalen
	MCUCR = 1<<ISC01;
	GICR = 1<<INT0; 
    //Interrupts global an
	sei();
	
    UCSRA = 0;
    UCSRB = (1<<TXEN);
    UBRRH = 0;
    UBRRL = 95;

    _delay_ms( 200 );
	uint8_t i = 1;
    while(1)
	{
        UDR = i;
        while(!(UCSRA&(1<<UDRE)));
		i <<= 1;
		if (!i)
		{
			i = 1;
		}
	    _delay_ms(200);
    }
    return 0;
}
