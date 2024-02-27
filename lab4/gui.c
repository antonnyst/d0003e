#include "gui.h"
#include "lcd.h"

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
// Runs pulsegenerators save/load
