#include <stdbool.h>
#include "pulsegenerator.h"
#include "TinyTimber.h"
#include "joystick.h"

#ifndef GUI_H
#define GUI_H

#define initGUI(left, right) { initObject(), 1, left, right, NULL}

typedef struct {
    Object super;
    int active; 
    PulseGenerator left;
    PulseGenerator right;
    Joystick *joystick;
} GUI;

int init(GUI *self);
int joystickEvent(GUI *self);

int joystickDown(GUI *self);
int joystickUp(GUI *self);

#endif