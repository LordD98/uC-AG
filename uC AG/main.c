/*
 * uC AG.c
 *
 * Created: 14.06.2017 18:17:19
 * Author : s-del
 */ 

#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	PORTC = 0xFF;
	DDRC = 0xFF;
    while (1) 
    {
		_delay_ms(300);
		PORTC ^= 0xFF;
    }
}

