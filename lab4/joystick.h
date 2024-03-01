#include <stdbool.h>
#include "pulsegenerator.h"
#include "TinyTimber.h"

#ifndef JOYSTICK_H
#define JOYSTICK_H

#define initJoystick(gui) { initObject(), gui, 0, NULL}

typedef struct {
    Object super;
    Object gui;
    int state;
    Msg last_event;
} Joystick;

int joystick_up(Joystick *self);
int joystick_down(Joystick *self);
int joystick_release(Joystick *self);

#endif