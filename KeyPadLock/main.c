#define F_CPU 14745600
#include <avr/io.h>
#include <util/delay.h>
#include "lcd-routines.h"

void InitKeyPad();  //<- Aufgabe 1
uint8_t ReadKeyPad(); //<-Aufgabe 2


void main()
{
    uint8_t i;
    uint8_t key;
    uint8_t passwd[9] = "12345678";     //Setze ein Passwort
    uint8_t userIn[9];                  //User Eingabe

	InitKeyPad();
	lcd_init();

	while(1)
	{
        lcd_clear();
        lcd_string("Key: ");
        lcd_setcursor(0,1);

        for(i=0; i<8; i++)
		{
            //Solange kein knopf gedrückt wird warten
			do 
			{
				key =  ReadKeyPad();
			}
			while(key == 0);
			
            if(key == 'B')
			{
                break;
            }
            else if(key != 'A')
			{
                //key in userIn speichern
				userIn[i]=key;
				userIn[i+1]='\0';
                //key auf lcd ausgeben
				//lcd_clear();
				lcd_setcursor(0,1);
				lcd_string(userIn);
            }
            //Solange noch ein Knopf gedrückt wird warten
			while(ReadKeyPad() != 0);
			_delay_ms(10);
        }

        if(i == 8)
		{
            for(i=0; i<8; i++)
			{
                if(passwd[i] != userIn[i])
                    break;
            }

            lcd_clear();
            if(i == 8)
			{
                lcd_string("Yes");
                lcd_setcursor(0,1);
                lcd_string("Bitch");
            }
            else
			{
                lcd_string("Fuck");
                lcd_setcursor(0,1);
                lcd_string("You");
            }
            _delay_ms(3000);
        }
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
	{'A','0','B'}};

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
