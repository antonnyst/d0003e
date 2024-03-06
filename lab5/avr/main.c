#include <avr/io.h>
#include "TinyTimber.h"
#include "lcd.h"

#define FOSC 8000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr) {
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 1stop bit */
    UCSR0C = 3<<UCSZ00;
}

int main() {

    CLKPR = 0x80;
	CLKPR = 0x00;

    USART_Init(MYUBRR);
    lcd_init();


    writeLong(0);

    while (!((UCSR0A & 0b10000000) > 0)) {
        //writeLong(UCSR0A);
    }

    int a = UDR0; // read data
    UDR0 = a;

    writeLong(a);
    //writeLong("123");
}

