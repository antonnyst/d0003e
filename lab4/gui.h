#include <stdbool.h>
#include "pulsegenerator.h"
#include "TinyTimber.h"

#ifndef GUI_H
#define GUI_H

#define initGUI(left, right) { initObject(), 0, left, right, 0, NULL}

typedef struct {
    Object super;
    int active; 
    PulseGenerator left;
    PulseGenerator right;
    int state;
    Msg last_event;
} GUI;

int start_gui(GUI *self);

int joystick_up(GUI *self);
int joystick_down(GUI *self);
int joystick_release(GUI *self);

int joystickLeft(GUI *self);
int joystickRight(GUI *self);
int joystickPress(GUI *self);

#endif