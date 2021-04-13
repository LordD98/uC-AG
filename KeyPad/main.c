#define F_CPU 14745600
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void InitKeyPad();  //<- Aufgabe 1
uint8_t ReadKeyPad(); //<-Aufgabe 2

int uart_put_char( char byte, FILE* stream);

FILE mystream = FDEV_SETUP_STREAM( uart_put_char, 0, _FDEV_SETUP_WRITE );

int main()
{
	//Init UART
	//.........................115200 Baud
	//.........................8 Bit
	//.........................1 Stoppbit
	//.........................no parity
	
	UBRRH = 0x00;
	UBRRL = 7;				// 7 für 115200b	95 9600

	UCSRA = 0x00;
	UCSRB = (1<<TXEN);
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);

    stdout = &mystream;

	InitKeyPad();
	while(1)
	{
		//Key lesen und mit printf schreiben
		printf("Keypad: %c\n", ReadKeyPad());
		//printf("x\n");
	}
}

void InitKeyPad()
{
    //PC0 bis PC3 als Output
    DDRC = 0x0F;
	//PC0 bis PC3 auf 0
	PORTC = 0x00;
}

uint8_t ReadKeyPad()
{
	uint8_t c, r; //Rows und Columns
	uint8_t input;
	uint8_t charSet[4][3] = {{'1','2','3'},
				 {'4','5','6'},
				 {'7','8','9'},
				 {'+','0','-'}};

	for(c=0; c<3; c++)
	{
		//Portc auf 0
		PORTC = 0x00;
		_delay_us(1000);
		if(c == 0)
		{
			//PC0 auf 1
			PORTC |= 1<<PC0;
		}
		else if(c == 1)
		{
			//PC1 auf 1
			PORTC |= 1<<PC1;
		}
		else if(c == 2)
		{
			//PC3 auf 1
			PORTC |= 1<<PC3;
		}
		_delay_us(1000);
		//input ist oberes Nibble von PINC geshiftet um 4 nach rechts
		input = PINC >> 4;
		if(input != 0)	// Oberes nibble != 0
			break;
	}

	if(c==3)
		return 0;

	r = 0;
	input >>= 1;
	while(input != 0)
	{
		input >>= 1;
		r++;
	}

	return charSet[r][c];
}

int uart_put_char( char byte, FILE* stream)
{
	while( !( UCSRA & (1<<UDRE ) ) );
	UDR = byte;
	return 0;
}
