/**
 * Program do zapisywania temperatury
 *
 * BL	    = PortC.1
 * Rs	    = PortC.2
 * E	    = PortC.3
 * Db4	    = PortC.4
 * Db5	    = PortC.5
 * Db6	    = PortC.6
 * Db7	    = PortC.7
 *
 * ds18b20  = PortD.4
 *
 * ind. led = PortB.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "lcd.h"

#define IND_LED_PORT   PORTB
#define IND_LED_DDR    DDRB
#define IND_LED_DQ     PD1

#define IND_LED_OUTPUT_MODE() IND_LED_DDR|=(1<<IND_LED_DQ)
#define IND_LED_ON()   IND_LED_PORT&=~(1<<IND_LED_DQ)
#define IND_LED_OFF()  IND_LED_PORT|=(1<<IND_LED_DQ)

#define THERM_PORT PORTD
#define THERM_DDR  DDRD
#define THERM_PIN  PIND
#define THERM_DQ   PD4

#define THERM_INPUT_MODE()  THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW()	    THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH()	    THERM_PORT|=(1<<THERM_DQ)

#define THERM_CMD_CONVERTTEMP	 0x44
#define THERM_CMD_RSCRATCHPAD	 0xbe
#define THERM_CMD_WSCRATCHPAD	 0x4e
#define THERM_CMD_CPYSCRATCHPAD  0x48
#define THERM_CMD_RECEEPROM	 0xb8
#define THERM_CMD_RPWRSUPPLY	 0xb4
#define THERM_CMD_SEARCHROM	 0xf0
#define THERM_CMD_READROM	 0x33
#define THERM_CMD_MATCHROM	 0x55
#define THERM_CMD_SKIPROM	 0xcc
#define THERM_CMD_ALARMSEARCH	 0xec

#define THERM_DECIMAL_STEPS_12BIT 625


static inline void delayloop16( uint16_t count ) {
	asm volatile (	"cp  %A0,__zero_reg__ \n\t"  \
			"cpc %B0,__zero_reg__ \n\t"  \
			"breq L_Exit_%=       \n\t"  \
			"L_LOOP_%=:	      \n\t"  \
			"sbiw %0,1	      \n\t"  \
			"brne L_LOOP_%=       \n\t"  \
			"L_Exit_%=:	      \n\t"  \
			: "=w" (count)
			: "0"  (count)
		     );				   
}

void delayloop32( uint32_t loops )  {
	asm volatile (	"cp  %A0,__zero_reg__ \n\t"  \
			"cpc %B0,__zero_reg__ \n\t"  \
			"cpc %C0,__zero_reg__ \n\t"  \
			"cpc %D0,__zero_reg__ \n\t"  \
			"breq L_Exit_%=       \n\t"  \
			"L_LOOP_%=:	      \n\t"  \
			"subi %A0,1	      \n\t"  \
			"sbci %B0,0	      \n\t"  \
			"sbci %C0,0	      \n\t"  \
			"sbci %D0,0	      \n\t"  \
			"brne L_LOOP_%=       \n\t"  \
			"L_Exit_%=:	      \n\t"  \
			: "=w" (loops)
			: "0"  (loops)
		     );

	return;
}

#define DELAY_MS_CONV(ms) ( (uint32_t) (ms*(F_OSC/6000L)) ) 
#define delay_ms(ms)  delayloop32(DELAY_MS_CONV(ms))
#define DELAY_US_CONV(us) ((uint16_t)(((((us)*1000L)/(1000000000/F_OSC))-1)/4))
#define delay_us(us)	  delayloop16(DELAY_US_CONV(us))

#define F_OSC 16000000UL

/**
 * reset czujnika temperatury
 */
uint8_t therm_reset() {
	uint8_t i;

	THERM_LOW();
	THERM_OUTPUT_MODE();
	delay_us( 480 );

	THERM_INPUT_MODE();
	delay_us( 60 );

	i = ( THERM_PIN & ( 1 << THERM_DQ ) );
	delay_us( 420 );

	return i;
}

/**
 * zapis bitu na wyjœcie termometru
 */
void therm_write_bit( uint8_t bit ) {
	THERM_LOW();
	THERM_OUTPUT_MODE();
	delay_us( 1 );

	if( bit ) THERM_INPUT_MODE();

	delay_us( 60 );
	THERM_INPUT_MODE();
}

/**
 * odczyt bitu z wyjœcia termometru
 */
uint8_t therm_read_bit( void ) {
	uint8_t bit = 0;

	THERM_LOW();
	THERM_OUTPUT_MODE();
	delay_us( 1 );

	THERM_INPUT_MODE();
	delay_us( 14 );

	if( THERM_PIN & ( 1 << THERM_DQ ) ) bit = 1;

	delay_us( 45 );

	return bit;
}

uint8_t therm_read_byte( void ) {
	uint8_t i = 8, n = 0;

	while( i-- ) {
		n >>= 1;
		n |= ( therm_read_bit() << 7 );
	}
	return n;
}

void therm_write_byte( uint8_t byte ) {
	uint8_t i = 8;

	while( i-- ) {
		therm_write_bit( byte&1 );
		byte >>= 1;
	}
}

/**
 * Metoda odczytuje temperaturê z czujnika do buffora znaków
 * @param *buffer buffor do którego wczytana zostanie temperatura
 */
void therm_read_temperature( char *buffer ) {
	uint8_t temperature[2];
	int8_t digit;
	uint8_t decimal;

	therm_reset();
	therm_write_byte( THERM_CMD_SKIPROM );
	therm_write_byte( THERM_CMD_CONVERTTEMP );

	while( !therm_read_bit() );

	therm_reset();
	therm_write_byte( THERM_CMD_SKIPROM );
	therm_write_byte( THERM_CMD_RSCRATCHPAD );

	temperature[0] = therm_read_byte();
	temperature[1] = therm_read_byte();
	therm_reset();

	digit = temperature[0] >> 4;
	digit |= ( temperature[1]&0x7 ) << 4;

	decimal = temperature[0]&0x7;
	decimal *= THERM_DECIMAL_STEPS_12BIT;

	sprintf( buffer, "%+d.%04d C", digit, decimal );
}

/**
 * Funkcja inicijalizuj¹ca ekran LCD
 */
void initLCD( void ) {
	LCD_init();		// inicjalizacja LCD
	LCD_PL_chars_init();	// inicjalizacja polskich znaków
}

/**
 * Metoda wy¿wietla wizytówkê
 */
void displayWelcomeMessage( void ) {
	LCD_putstr_P( PSTR( "Seiho Temp" ) );

	delayms( 500 );

	LCD_xy( 8, 1 );
	LCD_putstr_P( PSTR( "seiho.pl" ) );

	delayms( 1000 );
}

/**
 * Metoda wyœwietla temperaturê
 */
void displayTemp( void ) {

	IND_LED_ON();

	char buff[10];
	therm_read_temperature( buff );

	LCD_clear();
	LCD_putstr_P( PSTR( "Temp.: " ) );
	LCD_xy( 6, 1 );
	LCD_putstr( buff );

	IND_LED_OFF();

	delayms( 1000 );

}

/**
 * Metoda g³ówna
 */
int main( void ) {
	// inicjalizacja wyœwietlacza ...
	initLCD();

	// ustaw diode na output
	IND_LED_OUTPUT_MODE();
	IND_LED_OFF();

	// wyœwietl wizytówkê ...
	displayWelcomeMessage();

	// pêtla g³ówna
	while( 1 ) 
		displayTemp();

	return 0;
}

// vim: fdm=marker ts=4 sw=4 sts=4
