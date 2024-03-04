#include <stdbool.h>
#include "pulsegenerator.h"
#include "TinyTimber.h"
#include "gui.h"

#ifndef JOYSTICK_H
#define JOYSTICK_H

#define initJoystick(gui) { initObject(), gui }

typedef struct {
    Object super;
    GUI gui;
} Joystick;

int start_joystick(Joystick *self);
int joystickEventPCINT0(Joystick *self);
int joystickEventPCINT1(Joystick *self);

#endif