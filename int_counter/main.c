#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t count;

void setup();

int main(void)
{
    setup();

    while(1) {
    }

    return 0;
}

void setup() {
    count = 0;

    // PORTB als Ouput LEDs aus
	DDRB = 0xFF;
	PORTB = 0xFF;

    //INT1 PD3 als Output mit PUllup
	DDRD |= 1<<DDD3;
	PORTB 

    //INT1 lokal an auf fallende Flanke

    //Interrupts global an
}

ISR( /*HIER muss der richtige Vektor rein*/ ) {
    // count um 1 erhoehen

    // count auf PORTB ausgeben ( low acitve ! )

    //10 ms Warten wegen Prellen
}
