#include <avr/io.h>
#include "TinyTimber.h"
#include "lcd.h"
#include "serial.h"
#include "controller.h"
#include "interrupthandler.h"

int echo() {
    lcd_init();
    writeLong(0);

    while(true) {
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

    Controller controller = initController();

    GUI gui = initGUI(controller);

    InterruptHandler handler = initInterruptHandler(controller);

    INSTALL(&handler, handle_usart_rx, IRQ_USART0_RX);

    return TINYTIMBER(&gui, start_gui, NULL);

}

