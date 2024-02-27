#include "gui.h"
#include "lcd.h"
#include "pulsegenerator.h"

#define GUI_UPDATE_FREQ MSEC(50)

// INIT METHOD
// runs lcd_init() and starts update method
int init(GUI *self) {
    lcd_init();
    ASYNC(&self, update);
}


// UPDATE METHOD
// Updates the screen with actual values by checking with the objects
// Runs periodically
int update(GUI *self) {

    AFTER(GUI_UPDATE_FREQ, &self, update)
    return 0;
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
        ASYNC(self->left, increment, NULL);
        return 0;    
    }

    if (self->active == 1){ 
        ASYNC(self->right, increment, NULL);
        return 0;
    }

    else {
        return 1;
    }
}

int joystickDown(GUI *self){
    if (self->active == 0){
        ASYNC(self->left, decrement, NULL);
        return 0;

    }
    if (self->active == 1){ 
        ASYNC(self->right, decrement, NULL);
        return 0;
    }

    else {
        return 1;
    }
}

int joystickPress(GUI *self){
    if (self->active == 0){
        ASYNC(self->left, save_load, NULL);
        return 0;
    }

    if (self->active == 1){
        ASYNC(self->right, save_load, NULL);
        return 0;
    }

    else{
        return 1;
    }
}
////// HANDLERS ///////
// JOYSTICK LEFT
// Changes active to left

// JOYSTICK RIGHT
// Changes active to right

// JOYSTICK UP
// Runs pulsegenerators increase hz

// JOYSTICK DOWN
// Runs pulsegenerators decrease hz

// JOYSTICK PRESS
// Run pulsegenerators save/load
