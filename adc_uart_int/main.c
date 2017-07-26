
#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

//Standard uart putByte Methode
int uart_put_byte( uint8_t byte, FILE* stream );

//stdout stream fuer den uart
FILE uart_stream = FDEV_SETUP_STREAM( uart_put_byte, NULL, _FDEV_SETUP_WRITE );

//setup
void setup();


int main(void)
{
    setup();

    printf("Start\n");

    while(1);

    return 0;
}

void setup()
{
    //Port A als Input

    //  UART conifg
    //  * Senden und Empfangen
    //  * 9600 Baud keine Paritaet und 1 Stoppbit
	UBRRL = 95;
	UBRRH = 0;

	UCSRA = 0;
	UCSRB = (1<<TXEN) | (1<<RXEN);

	//TODO Recieve Interrupt fuer den UART enablen
	//  -> UCSRB Register modifizieren
	UCSRB |= (1<<RXCIE);

	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);

    //Umleiten des Streams
    stdout = &uart_stream;

    //TODO:
    //      *ADC enable
    //      *ADC im Single shot mode
    //      *ADC 128 prescaler
    //      *ADC Interrupt enaable
	
	ADCSRA = (1<<ADEN)|(7<<ADPS0)|(1<<ADIE);

	//TODO
	//      *ADC Quelle PA6
	//      *REF auf AVCC

	ADMUX = (6<<MUX0)|(1<<REFS0);


	// TODO Interrupts global freigeben
	sei();
}

ISR(ADC_vect)
{
	//TODO ADC value mit printf Ausgebe
	printf("Analog-wert: %u\n", (ADC>>2)&0xFF );
}

ISR(USART_RXC_vect)
{
    //TODO
    // Falls das Empfangene Zeichen '\n' ist soll der ADC gestartet werden
	if(UDR == '\n')
	{
		ADCSRA |= (1<<ADSC);
	}
}

int uart_put_byte( uint8_t byte, FILE* stream )
{
	while(!(UCSRA & (1<<UDRE)));
    UDR = byte;

	return 0;
}

