#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 14745600

void setup();
void UART_putByte(uint8_t byte);
uint8_t UART_getByte();




int main()
{
    uint8_t i;

	//Ruft setup
	setup();


	while(1) {
        for( i=1; i<10; i++ ) {
            UART_putByte( '0' + i);
            _delay_ms( 1000 );
        }
        UART_putByte( '1' );
        UART_putByte( '0' );
        _delay_ms( 1000 );
        UART_putByte( '\n');
	}


	return 0;
}


void setup(){

	//UBBRL und UBBRH mit dem Wert aus dem Datenblatt laden
	UBRRH = 0x00;
	UBRRL = 7;

	//->Even parity
	//->1 Stop Bit
	//->8 Bits an Daten

	//UCSRA laden
   	UCSRA = 0x00;

	//UCSRB laden
	UCSRB = (1<<RXEN) | (1<<TXEN);

	//UCSRC laden
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1) | (1<<UPM1);

}

void UART_putByte(uint8_t byte){

	//Warten bis UDR leer ist -> Bit in UCSRA
	while( !( UCSRA & ( 1<<UDRE )));
    //Daten in UDR laden
	UDR = byte;

}

uint8_t UART_getByte(){

	//Warten bis Empfangen komplett ist -> Bit in UCSRA
	while( !( UCSRA & ( 1<<RXC ) ) );
	//Daten aus UDR lesen

	return UDR;
}


