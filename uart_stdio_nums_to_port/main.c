#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define  F_CPU 14745600

void setup();

int uart_put_char( uint8_t byte, FILE* stream );

int uart_get_char( FILE* stream );

FILE mystream = FDEV_SETUP_STREAM( uart_put_char, uart_get_char, _FDEV_SETUP_RW );

int main(void) {

    uint16_t in;

    setup();

    stdout = &mystream;
    stdin = &mystream;

    while(1) {
        scanf("%d", &in);
        printf("Empfangen %d\n", in);

        PORTC = ~in;
        _delay_ms(150);
    }
    return 0;
}


void setup() {
    DDRC = 0xFF;

    UBRRH = 0x00;
    UBRRL = 7;

    UCSRA = 0x00;
    UCSRB = (1<<TXEN) | (1<<RXEN);
    UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
}

int uart_put_char( uint8_t byte, FILE* stream ) {
    while( !( UCSRA & (1<<UDRE ) ) );
    UDR = byte;
    return 0;
}

int uart_get_char( FILE* stream ) {
    while( !( UCSRA & (1<<RXC) ) );

    return UDR;
}



