#include <avr/io.h>
#include "gui.h"
#include "lcd.h"
#include "pulsegenerator.h"
#include "TinyTimber.h"

#define GUI_UPDATE_FREQ MSEC(500)

// UPDATE METHOD
// Updates the screen with actual values by checking with the objects
// Runs periodically
int update(GUI *self, int *arg) {

    //ASYNC(&(self->left), increment, 0);

    int left_hz = SYNC(&(self->left), get_hz, 0);
    int right_hz = SYNC(&(self->right), get_hz, 0);

    writeLong(left_hz);

    //printAt(left_hz, 0);
    //printAt(right_hz, 4);

    AFTER(CURRENT_OFFSET() + GUI_UPDATE_FREQ, self, update, 0);
    return 0;
}

void joystick_init() {
    PORTB |= 0b11010000;
    PORTE |= 0b00001100;

    PCMSK1 = 0b11010000;
    PCMSK0 = 0b00001100;

    EIMSK = 0b11000000;
}

// INIT METHOD
// runs lcd_init() and starts update method
int init(GUI *self) {
    lcd_init();
    joystick_init();
    ASYNC(self, update, 0);
}

int joystickLeft(GUI *self){
    //writeLong(1);

    self->active = 0;
    return 0;
}

int joystickRight(GUI *self){
    //writeLong(2);

    self->active = 1;
    return 0;
}

int joystickUp(GUI *self){
    //writeLong(3);


    if (self->active == 0){
        ASYNC(&(self->left), increment, 0);
        return 0;    
    }

    if (self->active == 1){ 
        ASYNC(&(self->right), increment, 0);
        return 0;
    }

    else {
        return 1;
    }
}

int joystickDown(GUI *self){
    //writeLong(4);

    if (self->active == 0){
        ASYNC(&(self->left), decrement, 0);
        return 0;

    }
    if (self->active == 1){ 
        ASYNC(&(self->right), decrement, 0);
        return 0;
    }

    else {
        return 1;
    }
}

int joystickPress(GUI *self){
    //writeLong(5);

    if (self->active == 0){
        ASYNC(&(self->left), save_load, 0);
        return 0;
    }

    if (self->active == 1){
        ASYNC(&(self->right), save_load, 0);
        return 0;
    }

    else{
        return 1;
    }
}

// it works
int joystickEvent(GUI *self){
    if (!(PINB & 0b10000000)){ // joystick is down
        joystickDown(self);
        return 0;
    }

    if (!(PINB & 0b01000000)){
        joystickUp(self);
        return 0;
    }

    if (!(PINE & 0b00000100)){
        joystickLeft(self);
        return 0;
    }

    if (!(PINE & 0b00001000)){
        joystickRight(self);
        return 0;
    }

    if (!(PINB & 0b00010000)){
        joystickPress(self);
        return 0;
    }
    return 1;
}
    
