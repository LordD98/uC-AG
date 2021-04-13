#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define data_size 12

uint8_t set_addr;
uint8_t data_addr;

/*
 * Registerbelegung:
 * Byte 0       LED 0
 * Byte 1       LED 1
 * Byte 2       Button 0
 * Byte 3       Button 1
 * Byte 4..5    ADC0
 * Byte 6..7    ADC1
 * Byte 8..9    PWM TOP (Periode)
 * Byte 10..11  PWM OCR (Duty Cycle)
 */
uint8_t data_register[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void slave_setup(uint8_t addr);
void slave_recieve( );
void slave_load_data( );
void slave_handle_data(uint8_t data);
void slave_status_handler( );

void IO_setup();
void IO_update();

int main()
{
    data_addr = 0;
    set_addr = 1;
    
    slave_setup(0x02);
    IO_setup();
    
    while(1)
	{
		// PORTB ^= 1<<PB0;
		// PORTD ^= 1<<PD7;
		// _delay_ms(500);
		// }
		// {
		
        while(set_addr)
		{
            slave_recieve();
            IO_update();
        }
        while(!set_addr)
		{
            slave_recieve();
            IO_update();
        }
    }
}


void slave_setup(uint8_t addr)
{
    //setup the TWI with 1024Hz SCK
	//! 4 & 28;
	TWAR = (addr<<1)
	TWBR = 16;
	TWCR = (1<<TWEN)|(1<<TWEA);
}

void slave_recieve( )
{
    //reset interrupt flag
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    //wait for next byte
    while (!(TWCR&(1<<TWINT)));
    //handle TWI status
    slave_status_handler();
}


void slave_load_data()
{
    //load next dataelement
    TWDR = data_register[data_addr];
    //increment data address with wrap around
    data_addr++;
	data_addr%=data_size;
}


void slave_handle_data(uint8_t data)
{
    if(set_addr)
	{
        //reset set_addr and set data address to new value    
        
    }
	else
	{
        //write new data to register
        
        //increment data address with wrap around
        
    }
}


void slave_status_handler( )
{
    switch(TWSR & 0xF8)
	{
        //Slave Recieve Mode
        case 0x60:  //Own SLA+W recieved; ACK returned
        case 0x68:  //Own SLA+W recieved; ACK returned
        case 0x70:  //Genral call recieved; ACK returned
        case 0x78:  //Genral call recieved; ACK returned
            break;
            
        case 0x80:  //Adressed with SLA+W; data recieved; ACK returned
        case 0x88:  //Adressed with SLA+W; data recieved; NACK returned
        case 0x90:  //Adressed with Gerneal Call; data recieved; ACK returned
        case 0x98:  //Adressed with Genral Call; data recieved; NACK returned
            //handle recieved data
            slave_handle_data(TWDR);
            break;
            
        case 0xA0:  //STOP or rep. START recieved while addressed als slave
            //set set_addr on STOP and RESTART
            set_addr = 1;
            break;    
        
        //Slave Transmit Mode
        case 0xA8:  //Own SLA+R recieved; ACK returned
        case 0xB0:  //Own SLA+R recieved; ACK returned
        case 0xB8:  //Data transmitted; ACK recieved
            //send data
            slave_load_data();
            break;
            
        case 0xC0:  //Data transmitted; NACK recieved 
        case 0xC8:  //Last data transmitted (TWEA=0); ACK recieved
        default:
            break;
    }
    //clear interrupt flag and set ACK bit
    TWCR |= (1<<TWINT) | (1 << TWEA);
}

void IO_setup()
{    
    //LED 0 -> PB0
    //LED 1 -> PD7
	PORTB |= (1<<PB0);
	PORTD |= (1<<PD7);
    DDRB |= (1<<PB0);
	DDRD |= (1<<PD7);
}

void IO_update()
{
    //write data registers to LED PORTs
	if(data_register[0])
	{
		
	}   
	if (data_register[1])
	{
	}
}
