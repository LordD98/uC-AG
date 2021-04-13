
// Ansteuerung eines HD44780
#include <avr/io.h>
#include "lcd-routines.h"
#include <util/delay.h>

////////////////////////////////////////////////////////////////////////////////
// Erzeugt einen Enable-Puls
static void lcd_enable(void)
{
    //  1) Setze LCD_EN im LCD_PORT auf 1
	LCD_PORT |= (1<<LCD_EN);
    //*   2) Warte LCD_ENABLE_US
	_delay_us(LCD_ENABLE_US);
    //*   3) Setze LCD_EN im LCD_PORT auf 0
	LCD_PORT &= ~(1<<LCD_EN);
}

////////////////////////////////////////////////////////////////////////////////
// Sendet eine 4-bit Ausgabeoperation an das LCD
static void lcd_out(uint8_t data)
{
    data &= 0xF0;              // obere 4 Bit maskieren
    LCD_PORT &= ~(0xF0>>1);    // Maske löschen
    LCD_PORT |= (data>>1);     // Bits setzen
    lcd_enable();
}

////////////////////////////////////////////////////////////////////////////////
// Initialisierung: muss ganz am Anfang des Programms aufgerufen werden.
void lcd_init( void )
{
    // verwendete Pins auf Ausgang schalten
    uint8_t pins = (0xF0>>1) |           // 4 Datenleitungen
                   (1<<LCD_RS) |                // R/S Leitung
                   (1<<LCD_EN) |                // Enable Leitung
                    (1<<LCD_RW);
    LCD_DDR |= pins;

    // initial alle Ausgänge auf Null
    LCD_PORT &= ~pins;

    // warten auf die Bereitschaft des LCD
    _delay_ms(LCD_BOOTUP_MS);

    // Soft-Reset muss 3mal hintereinander gesendet werden zur Initialisierung
    lcd_out(LCD_SOFT_RESET);
    _delay_ms(LCD_SOFT_RESET_MS1);

    lcd_enable();
    _delay_ms(LCD_SOFT_RESET_MS2);

    lcd_enable();
    _delay_ms(LCD_SOFT_RESET_MS3);

    // 4-bit Modus aktivieren
    lcd_out(LCD_SET_FUNCTION | LCD_FUNCTION_4BIT);
    _delay_ms(LCD_SET_4BITMODE_MS);

    // 4-bit Modus / 2 Zeilen / 5x7
    lcd_command(LCD_SET_FUNCTION | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINE | LCD_FUNCTION_5X7);

    // Display ein / Cursor aus / Blinken aus
    lcd_command(LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_BLINKING_ON);

    // Cursor inkrement / kein Scrollen
    lcd_command(LCD_SET_ENTRY | LCD_ENTRY_INCREASE | LCD_ENTRY_NOSHIFT);

    lcd_clear();
}

////////////////////////////////////////////////////////////////////////////////
// Sendet ein Datenbyte an das LCD
void lcd_data(uint8_t data)
{
    LCD_PORT |= (1<<LCD_RS);    // RS auf 1 setzen

    lcd_out(data);            // zuerst die oberen,
    lcd_out(data<<4);         // dann die unteren 4 Bit senden

    _delay_us(LCD_WRITEDATA_US);
}

////////////////////////////////////////////////////////////////////////////////
// Sendet einen Befehl an das LCD
void lcd_command(uint8_t data)
{
    // 1) Setzte LCD_RS auf 0
	LCD_PORT &= ~(1<<LCD_RS);
    // 2) sende das obere Nibble
	lcd_out(data);
    // 3) sende das untere Nibble
	lcd_out(data << 4);
    // 4) warte LCD_COMMAND_US
	_delay_us(LCD_COMMAND_US);
    // Tipp: benutze lcd_out() zum senden */
}

////////////////////////////////////////////////////////////////////////////////
// Sendet den Befehl zur Löschung des Displays
void lcd_clear(void)
{
    lcd_command(LCD_CLEAR_DISPLAY);
    _delay_ms(LCD_CLEAR_DISPLAY_MS);
}

////////////////////////////////////////////////////////////////////////////////
// Sendet den Befehl: Cursor Home
void lcd_home(void)
{
    lcd_command(LCD_CURSOR_HOME);
    _delay_ms(LCD_CURSOR_HOME_MS);
}

////////////////////////////////////////////////////////////////////////////////
// Setzt den Cursor in Spalte x (0,7) Zeile y (1,2)

void lcd_setcursor(uint8_t x, uint8_t y)
{
    uint8_t data;

    switch (y)
    {
        case 0:    // 1. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE1 + x;
            break;

        case 1:    // 2. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE2 + x;
            break;

        default:
            return;  // für den Fall einer falschen Zeile
    }

    lcd_command(data);
}

////////////////////////////////////////////////////////////////////////////////
// Schreibt einen String auf das LCD

void lcd_string(const char *data)
{
    /* 1) Iteriere durch das Array und sende jedes einzelne Zeichen mit lcd_data
    *      Strings enden immer mit '\0' */
	char *c = &data[0];
	while(*c != '\0')
	{
		lcd_data(*c);
		c++;
	}
}
