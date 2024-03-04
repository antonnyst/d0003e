#include <avr/io.h>
#include "gui.h"
#include "lcd.h"
#include "pulsegenerator.h"
#include "TinyTimber.h"
#include "joystick.h"

#define GUI_UPDATE_FREQ MSEC(50)

#define JOYSTICK_REPEAT MSEC(100)
#define JOYSTICK_DELAY  MSEC(600)

// UPDATE METHOD
// Updates the screen with actual values by checking with the objects
// Runs periodically
int update(GUI *self, int *arg) {

    int left_hz = SYNC(&(self->left), get_hz, 0);
    int right_hz = SYNC(&(self->right), get_hz, 0);

    printAt(left_hz, 0);
    printAt(right_hz, 4);

    if (self->active == 0) {
        LCDDR0 |= 0b00100010;
    } else if (self->active == 1) {
        LCDDR2 |= 0b00100010;        
    }

    AFTER(CURRENT_OFFSET() + GUI_UPDATE_FREQ, self, update, 0);
    return 0;
}



// INIT METHOD
// runs lcd_init() and starts update method
int start_gui(GUI *self) {
    lcd_init();

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

int repeat(GUI *self) {
    
    if (self->state == 1) {
        ASYNC(self, joystickUp, NULL);
    } else if (self->state == 2) {
        ASYNC(self, joystickDown, NULL);
    }

    if (self->state > 0) {
        AFTER(CURRENT_OFFSET() + JOYSTICK_REPEAT, self, repeat, NULL);
    }
    return 0;
}

int joystick_up(GUI *self) {
    if (self->state == 0) {
        self->state = 1;
        self->last_event = AFTER(CURRENT_OFFSET()+JOYSTICK_DELAY, self, repeat, NULL);
        ASYNC(self, joystickUp, NULL);
        return 0;
    }
}

int joystick_down(GUI *self) {
    if (self->state == 0) {
        self->state = 2;
        self->last_event = AFTER(CURRENT_OFFSET()+JOYSTICK_DELAY, self, repeat, NULL);
        ASYNC(self, joystickDown, NULL);
    }
    return 0;
}

int joystick_release(GUI *self) {
    if (self->state > 0) {
        self->state = 0;
        ABORT(self->last_event);
        self->last_event = NULL;
    }
    return 0;
}