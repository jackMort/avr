#define F_CPU		16000000 	// cz�stotliwo�� zegara w Hz

//----------------------
// I2C
//----------------------
#define I2C_PORT 	PORTC	//port do kt�rego s� pod��czone linie magistrali
#define I2C_SDA		1	//bit powy�szego portu u�ywany jako linia SDA
#define I2C_SCL		0	//bit powy�szego portu u�ywany jako linia SCL

/////////////////////////////////////////////////////////////////////////

//----------------------
// PCF8583
//----------------------

#define PCF8583_A0	1	//stan linii A0 uk�adu (adres na magistrali)

/////////////////////////////////////////////////////////////////////////

//----------------------
// UART
//----------------------
#define UART_BAUD	9600		// pr�dko�� transmisji
#define UART_BUF_SIZE	16		// wielko�� bufor�w UART 
#define UART_MAX_GETSTR	8	

//----------------------
// LCD 
//----------------------

#define LCD_PORT	PORTC	//port wy�wietlacza
#define LCD_RS 		2	//bit linii RS
#define LCD_EN 		3	//bit linii EN
#define LCD_X		16	//liczba znak�w w linii wy�wietlacza
#define LCD_Y		2	//liczba wierszy wy�wietlacza
#define LCD_WRAP		//zawijanie tekstu
#define LCD_WIN1250		//polskie znaki wg WIN1250

