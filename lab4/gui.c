#include <avr/io.h>
#include "gui.h"
#include "lcd.h"
#include "pulsegenerator.h"
#include "TinyTimber.h"
#include "joystick.h"

#define GUI_UPDATE_FREQ MSEC(500)


// UPDATE METHOD
// Updates the screen with actual values by checking with the objects
// Runs periodically
int update(GUI *self, int *arg) {

    int left_hz = SYNC(&(self->left), get_hz, 0);
    int right_hz = SYNC(&(self->right), get_hz, 0);

    printAt(left_hz, 0);
    printAt(right_hz, 4);

    if (self->active == 0) {
        LCDDR0 |= 0b00000010;
    } else if (self->active == 1) {
        LCDDR2 |= 0b00100000;        
    }

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

    Joystick j = initJoystick(self);

    self->joystick = &j;

    // Pin init
    DDRE = 0b11110011;

    ASYNC(self, update, 0);
}


// works
int joystickLeft(GUI *self){
    //writeLong(1);

    self->active = 0;
    return 0;
}

// works
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
        return 0;
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
        return 0;
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
        return 0;
    }
}

// 
// Object Joystick 
// int = Up inget down
// Msg last_event

// repeat
// continously check enum

// joystick_up
// send start repeat in delay
//
// joystick_release
// abort last_event
//
// joystick_down
// send start repeat in delay
//


// it works
int joystickEvent(GUI *self){
    if (!(PINB & 0b10000000)){ // joystick is down
        writeLong(22);
        ASYNC(self->joystick, joystick_down, NULL);
        return 0;
    } else if (PINB == 0b11111111 || PINE == 0b11111111) {
        //ASYNC(self->joystick, joystick_release, NULL);
    }

    if (!(PINB & 0b01000000)){
        writeLong(11);
        ASYNC(self->joystick, joystick_up, NULL);
        return 0;
    } else if (PINB == 0b11111111 || PINE == 0b11111111) {
        //ASYNC(self->joystick, joystick_release, NULL);
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
    return 0;
}
    
