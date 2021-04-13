#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>

#define MW  0x00

#define MW_START_TRANSMIT   0x08
#define MW_RSTART_TRANSMIT  0x10
#define MW_SLAW_ACK         0x18
#define MW_SLAW_NACK        0x20
#define MW_DATA_ACK         0x28
#define MW_DATA_NACK        0x30
#define MW_ARB_LOST         0x38

#define MR  0x01
#define MR_START_TRANSMIT   0x08
#define MR_RSTART_TRANSMIT  0x10
#define MR_ARBLOST_NACK     0x38
#define MR_SLAR_ACK         0x40
#define MR_SLAR_NACK        0x48
#define MR_DATA_ACK         0x50
#define MR_DATA_NACK        0x58

/*Registerbelegung:
 * Byte 0       LED 0
 * Byte 1       LED 1
 * Byte 2       Button 0
 * Byte 3       Button 1
 * Byte 4..5    ADC0
 * Byte 6..7    ADC1
 * Byte 8..9    PWM TOP (Periode)
 * Byte 10..11  PWM OCR (Duty Cycle)
 */

void twi_write_reg( uint8_t addr, uint8_t reg, uint8_t length, uint8_t *data )
{
    uint8_t status;
    uint8_t i;

    //Start condition send
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR&(1<<TWINT)));
	
    status = TWSR & 0xF8;
    if( status != MW_START_TRANSMIT )
	{
        //Error
    }

    //Addr plus rw send
	TWDR = (addr<<1)|MW;
	TWCR = (1<<TWINT)|(1<<TWEN)

    status = TWSR & 0xF8;
    if( status != MW_SLAW_ACK )
	{
        //Error
    }

    //Sendin REG addr
	TWDR = reg;
	TWCR = (1<<TWINT)|(1<<TWEN);
	
    status = TWSR & 0xF8;
    if( status != MW_DATA_ACK )
	{
        //Error
    }

    //Data send
    for( i=0; i<length; i++ )
	{
        //write byte
        TWDR = *data;
        data++;
        //send byte
		TWCR = (1<<TWINT)|(1<<TWEN);
        status = TWSR & 0xF8;
        if( status != MW_DATA_ACK )
		{
            //Error
        }
    }

    //Send Stop
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

uint8_t twi_read_reg( uint8_t addr, uint8_t reg, uint8_t length, uint8_t *data )
{
    uint8_t status;
    uint8_t i;

    //Start condition send
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!());

    status = TWSR & 0xF8;
    if( status != MW_START_TRANSMIT )
	{
        //Error
    }

    //Addr plus rw send

    status = TWSR & 0xF8;
    if( status != MW_SLAW_ACK )
	{
        //Error
    }

    //Sendin REG addr

    status = TWSR & 0xF8;
    if( status != MW_DATA_ACK )
	{
        //Error
    }

    //Repeated Start condition send
	TWCR = (1<<TWEN)|(1<<TWSTA)|(1<<TWINT);
    status = TWSR & 0xF8;
    if( status != MR_RSTART_TRANSMIT )
	{
        //Error
    }

    //Addr plus rw send

    status = TWSR & 0xF8;
    if( status != MR_SLAR_ACK )
	{
        //Error
    }

    //Data rec
    for( i=0; i<length-1; i++ )
	{
        //wait for next byte
		TWCR = (1<<TWEA);
		
        status = TWSR & 0xF8;
        if( status != MW_DATA_ACK )
		{
            //Error
        }
        //read byte
        *data = TWDR;
		data++;
    }

    //wait for last byte
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR&(1<<TWINT)));	
    status = TWSR & 0xF8;
    if( status != MW_DATA_ACK )
	{
            //Error
    }
    *data = TWDR;

	TWCR = (1<<TWSTO)|(1<<TWEN)|(1<<TWINT);
    //Send Stop
}

void uart_write( uint8_t byte )
{
	while( !( UCSRA & (1<<UDRE)));
	UDR = byte;
}

int main(void)
{
    uint8_t i, j;
    uint8_t LED[2] = {0,1};
    uint8_t BUTTON[2] = {0,0};

    UCSRA = 0;
    UCSRB = (1<<TXEN);
    UCSRC = (1<<UCSZ0) | (1<<UCSZ1);
    UBRRL = 95;
    UBRRH =0;

    //init TWI with 1024Hz SCK
	TWBR = 16;
	TWCR = 0;
	TWSR = 0;
	
    DDRB = 0xFF;
    PORTB = 0xFF;

	while(1)
	{
            //LEDs toggeln
            LED[0] ^= 0x01;
            LED[1] ^= 0x01;
            twi_write_reg(0x02, 0x00, 2, LED);
            _delay_ms(50);
            
            //LEDs einlesen und auf PORTB ausgeben
            twi_read_reg( 0x02, 0x00, 2, BUTTON);
            
            if(BUTTON[0])
                PORTB |= 0x0F;
            else
                PORTB &= ~0x0F;
            
            if(BUTTON[1])
                PORTB |= 0xF0;
            else
                PORTB &= ~0xF0;
            
            _delay_ms( 500 );
	}
    return 0;
}
