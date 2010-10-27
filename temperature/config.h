#define F_CPU		16000000 	// częstotliwoć zegara w Hz

//----------------------
// I2C
//----------------------
#define I2C_PORT 	PORTC	//port do którego sš podłšczone linie magistrali
#define I2C_SDA		1	//bit powyższego portu używany jako linia SDA
#define I2C_SCL		0	//bit powyższego portu używany jako linia SCL

/////////////////////////////////////////////////////////////////////////

//----------------------
// PCF8583
//----------------------

#define PCF8583_A0	1	//stan linii A0 układu (adres na magistrali)

/////////////////////////////////////////////////////////////////////////

//----------------------
// UART
//----------------------
#define UART_BAUD	9600		// prędkoć transmisji
#define UART_BUF_SIZE	16		// wielkoć buforów UART 
#define UART_MAX_GETSTR	8	

//----------------------
// LCD 
//----------------------

#define LCD_PORT	PORTC	//port wywietlacza
#define LCD_RS 		2	//bit linii RS
#define LCD_EN 		3	//bit linii EN
#define LCD_X		16	//liczba znaków w linii wywietlacza
#define LCD_Y		2	//liczba wierszy wywietlacza
#define LCD_WRAP		//zawijanie tekstu
#define LCD_WIN1250		//polskie znaki wg WIN1250

