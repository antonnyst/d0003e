#include <stdbool.h>
#include "TinyTimber.h"
#include "controller.h"

#ifndef GUI_H
#define GUI_H

#define initGUI(controller) { initObject(), controller }

typedef struct {
    Object super;
    Controller *controller;
} GUI;

int start_gui(GUI *self);

#endif