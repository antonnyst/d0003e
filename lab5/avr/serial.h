#include "TinyTimber.h"

#ifndef SERIAL_H
#define SERIAL_H

#define initSerial(left, right) { initObject() }

typedef struct {
    Object super; 
} Serial;

int start_serial(Serial *self);


#endif