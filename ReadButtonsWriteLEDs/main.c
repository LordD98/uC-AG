
#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>

uint8_t scanButtons();
void writeLEDs(uint8_t val);


int main(void)
{
	PORTA = 0xFF;	// Pullups auf PORTA
	PORTC = 0xFF;
	DDRC = 0xFF;
	
	//Buttons auf PORTA
	
	while (1)
	{
		writeLEDs(scanButtons());
		_delay_ms(100);
	}
}

uint8_t scanButtons()
{
	return (~PINA) & 0x1F;
}

void writeLEDs(uint8_t val)
{
	PORTC = ~(val);
}