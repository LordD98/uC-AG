
#include <avr/io.h>
#include <avr/interrupt.h>

ISR( TIMER1_COMPA_vect )
{
    PORTB ^= (1<<PB5);
}

int main(void)
{
    DDRB = (1<<PB5);
    PORTB = 0;

    TCNT1 = 0;
    TCCR1A = 0;
    TCCR1B = (1<<CS11) | (1<<CS10) | (1<<WGM12);
    TIMSK = (1<<OCIE1A);
    OCR1A = 46080;
    sei();
    while(1);
}
