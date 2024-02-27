#include <stdbool.h>
#include "pulsegenerator.h"

#ifndef GUI_H
#define GUI_H

#define initGUI(left, right) {false, left, right}

typedef struct {
    bool active; 
    PulseGenerator left;
    PulseGenerator right;
} GUI;

int init(GUI *self);
int joystickEvent(GUI *self);


#endif