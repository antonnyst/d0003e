#include "joystick.h"
#include "TinyTimber.h"
#include "gui.h"
#include "lcd.h"

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

#define JOYSTICK_REPEAT MSEC(50)
#define JOYSTICK_DELAY  MSEC(500)

int repeat(Joystick *self) {
    
    if (self->state == 1) {
        ASYNC(&(self->gui), joystickUp, NULL);
    } else if (self->state == 2) {
        ASYNC(&(self->gui), joystickDown, NULL);
    }

    if (self->state > 0) {
        AFTER(CURRENT_OFFSET() + JOYSTICK_REPEAT, self, repeat, NULL);
    }
    return 0;
}

int joystick_up(Joystick *self) {
    self->state = 1;
    self->last_event = AFTER(CURRENT_OFFSET()+JOYSTICK_DELAY, self, repeat, NULL);
    ASYNC(&(self->gui), joystickUp, NULL);
    return 0;
}

int joystick_down(Joystick *self) {
    self->state = 2;
    self->last_event = AFTER(CURRENT_OFFSET()+JOYSTICK_DELAY, self, repeat, NULL);
    ASYNC(&(self->gui), joystickDown, NULL);
    return 0;
}

int joystick_release(Joystick *self) {
    self->state = 0;
    ABORT(self->last_event);
    self->last_event = NULL;
    return 0;
}

