#include <avr/io.h>
#include <stdint.h>
#include "serial.h"
#include "TinyTimber.h"

#define FOSC 8000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int start_serial(Serial *self) {
    /* Set baud rate */
    UBRR0H = (unsigned char)(MYUBRR>>8);
    UBRR0L = (unsigned char)MYUBRR;
    /* Enable receiver and transmitter + reciever interrupts */
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
    /* Set frame format: 8data, 1stop bit */
    UCSR0C = 3<<UCSZ00;

    return 0;
}

int serial_write(Serial *self, uint8_t data) {

    // Wait until we can write
    while ((UCSR0A & 0b00100000) == 0) {}

    // Write data
    UDR0 = data;

    return 0;
}

int serial_read(Serial *self) {
    
    // Wait until we can read
    while ((UCSR0A & 0b10000000) == 0) {}

    // Read data
    int data = UDR0;

    return data;
}
