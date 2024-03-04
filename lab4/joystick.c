#include <avr/io.h>
#include "joystick.h"
#include "TinyTimber.h"
#include "gui.h"
#include "lcd.h"

void joystick_init() {
    PORTB |= 0b11010000;
    PORTE |= 0b00001100;

    PCMSK1 = 0b11010000;
    PCMSK0 = 0b00001100;

    EIMSK = 0b11000000;
}

int start_joystick(Joystick *self) {
    joystick_init();
    ASYNC(&(self->gui), start_gui, NULL);
}

int joystickEventPCINT0(Joystick *self){
    if (!(PINE & 0b00000100)){
        ASYNC(&(self->gui), joystickLeft, NULL);
        return 0;
    } else if (!(PINE & 0b00001000)){
        ASYNC(&(self->gui), joystickRight, NULL);
        return 0;
    } 
}

int joystickEventPCINT1(Joystick *self){
    if (!(PINB & 0b10000000)){ // joystick is down
        ASYNC(&(self->gui), joystick_down, NULL);
        return 0;
    } else if (!(PINB & 0b01000000)){
        ASYNC(&(self->gui), joystick_up, NULL);
        return 0;
    } else if (!(PINB & 0b00010000)){
        ASYNC(&(self->gui), joystickPress, NULL);
        return 0;
    } else {
        ASYNC(&(self->gui), joystick_release, NULL);
        return 0;
    }
}