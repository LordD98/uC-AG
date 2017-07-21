
#define F_CPU 14745600

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>


int uart_put_byte(uint8_t byte, FILE* stream);

FILE mystream = FDEV_SETUP_STREAM( uart_put_byte, NULL, _FDEV_SETUP_WRITE );

void setup();

uint16_t read_adc();

int main(void)
{
    uint16_t adc_value;
    setup();
    while(1)
	{
	    if((~PINA & (1<<PINA0)))
		{
			adc_value = read_adc();
			/*adc_value mit printf ausgeben*/
			printf("Analog-Wert: %d\n", (adc_value>>2)&0xFF);
			/*10ms Warten zum entprellen*/
			_delay_ms(10);
	    }
    }

    return 0;
}

void setup() {
	/*
	 * UART mit 9600 Baud no Parity und 1 Stoppbit
	 */
	UBRRH = 0;
	UBRRL = 95;

	UCSRA = 0;
	UCSRB = (1<<TXEN);
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);


	stdout = &mystream;
	stdin = &mystream;
	
	printf("ADC Beispiel mcAG \n");
	printf("=========================================================================\n");


	//Gesamter PORTA als Input
	DDRA = 0;

	//PA0 Pullup
	PORTA |= 1<<PA0; 
	//ADCMUX auf Source 6 mit Referenz AVCC
	
	//ADCR
	// * ADC enablen
	// * ADC kein Auto Trigger
	// * ADC Teiler auf 128
	
	ADMUX = (1<<REFS0)|(7<<MUX0);
	//SFIOR = 0; //free running mode
	ADCSRA = (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
	//ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE);
}

uint16_t read_adc() {
	//ADCR Conversation starten
	ADCSRA |= (1<< ADSC);
	while (ADCSRA&(1<<ADSC));
	return ADC;
}

int uart_put_byte(uint8_t byte, FILE* stream)
{
	while(!(UCSRA&(1<<UDRE)));
	UDR = byte;
	return 0;
}
