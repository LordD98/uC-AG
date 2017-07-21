
#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t count;

void setup(void);

int main(void)
{
    setup();

    while(1);

    return 0;
}

void setup()
{
    count = 0;

    // PORTB als Ouput LEDs aus
	DDRB = 0xFF;
	PORTB = 0xFF;

    //INT1 PD3 als Input mit Pullup
	//DDRD &= ~1<<DDD3;
	PORTD |= 1<<PD3;

    //INT1 lokal an auf fallende Flanke
	MCUCR |= 1<<ISC11;
	GICR |= 1<<INT1;

    //Interrupts global an
	sei();
}

ISR(INT1_vect)
{
    // count um 1 erhoehen
	count++;
    // count auf PORTB ausgeben ( low acitve ! )
	PORTB = ~count;
    //10 ms Warten wegen Prellen
	_delay_ms(10);
}
