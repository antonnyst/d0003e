
#include <stdbool.h>
#include "TinyTimber.h"
#include "serial.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define initController(serial) { initObject(), serial, 0, 0, 0, 0, 0, NONE }

enum Direction {
    NONE,
    NORTH,
    SOUTH
};

typedef struct {
    Object super;
    Serial serial;
    int north_amt;
    int bridge_amt;
    int south_amt;

    int sent;
    int wait;
    enum Direction direction;
} Controller;

int start_controller(Controller *self);

int handle_recieve(Controller *self);

int get_north_amt(Controller *self);
int get_bridge_amt(Controller *self);
int get_south_amt(Controller *self);

#endif