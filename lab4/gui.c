#include <avr/io.h>
#include "gui.h"
#include "lcd.h"
#include "pulsegenerator.h"
#include "TinyTimber.h"

#define GUI_UPDATE_FREQ MSEC(50)

// UPDATE METHOD
// Updates the screen with actual values by checking with the objects
// Runs periodically
int update(GUI *self, int *arg) {


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
    self->active = 0;
    return 0;
}

int joystickRight(GUI *self){
    self->active = 0;
    return 0;
}

int joystickUP(GUI *self){
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

