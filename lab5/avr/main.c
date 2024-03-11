#include <avr/io.h>
#include "TinyTimber.h"
#include "gui.h"
#include "lcd.h"
#include "serial.h"
#include "controller.h"
#include "interrupthandler.h"

#define FOSC 8000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int USART_Init(unsigned int ubrr) {
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter + reciever interrupts */
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);//| (1<<RXCIE0);
    /* Set frame format: 8data, 1stop bit  */
    UCSR0C = 3<<UCSZ00;

    return 0;
}

int echo() {
    USART_Init(MYUBRR);
    lcd_init();
    writeLong(0);
    while(1) {
        while (!((UCSR0A & 0b10000000) > 0)) {
            //writeLong(UCSR0A);
        }

        int a = UDR0; // read data
        UDR0 = a;

        writeLong(a);
    }
    
}

int main() {

    CLKPR = 0x80;
	CLKPR = 0x00;

    //echo();

    ///*
    Serial serial = initSerial();

    Controller controller = initController(serial);

    GUI gui = initGUI(&controller);

    InterruptHandler handler = initInterruptHandler(&controller);

    INSTALL(&controller, handle_recieve, IRQ_USART0_RX);

    return TINYTIMBER(&gui, start_gui, NULL);
    //*/
}

