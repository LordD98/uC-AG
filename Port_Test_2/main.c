
#define F_CPU 14745600UL
#define AUTO 1
#define  MANUAL 0

#include <avr/io.h>
#include <util/delay.h>



/**
 * Aufbau:
 * 	+Spielwiese I an PORTA / PORTC
 *
 * Aufgaben:
 * 	1.Setze PORTC als Ouput, PORTA als Input und die Pull-Ups für PA0..PA4
 * 	2.Lass die LED mittels shift in einer Schleife in eine Richtung wandern
 * 	3.hin und her
 *  4.Nutye die Taster 0, 1 auf PORTA um zwischen links, rechts zu wechseln
 *
 *  5.Passe die Geschwindigkeit mit den Tastern 2, 3 an
 *
 * 	Nützlich:
 * 		DDR, PORT, for, _delay_...
 **/

void handleLeds(void);
void handleButtons(void);

unsigned char count = 0;
signed char dir = 1;
unsigned char mode = AUTO;

int main(void)
{
	//Aufgabe 1
	DDRC = 0xFF;
	PORTC = 0xFF;
	DDRA = 0x00;
	PORTA = 0x0F;	
	//Aufgabe 2
    while (1)
    {
		handleButtons();
		handleLeds();
	}
    return 0;
}

void handleButtons()
{
	if (!(PINA & 1<<PINA0))
	{
		if (!(PINA & 1<<PINA1))
		{
			while (!(PINA & 1<<PINA0) || !(PINA & 1<<PINA1)) handleLeds();
			mode = AUTO;
		}
		else
		{
			dir = -1;
			mode = MANUAL;
		}
	} 
	else if (!(PINA & 1<<PINA1))
	{
		dir = 1;
		mode = MANUAL;
	}
	
}

void handleLeds()
{
	PORTC = ~(1<<count);
	count += dir;
	//Aufgabe 3
	if (mode == AUTO)
	{
		if(count == 7 || count == 0)
		dir *= -1;
	}
	else	//mode == MANUAL
	{
		if(count == 8)
			count = 0;
		else if(count == 255)
			count = 7;	
	}
	_delay_ms(300);
	
}