#include "TinyTimber.h"

#ifndef SERIAL_H
#define SERIAL_H

#define initSerial() { initObject() }

typedef struct {
    Object super; 
} Serial;

int start_serial(Serial *self);
int serial_write(Serial *self, int data);
int serial_read(Serial *self);

#endif