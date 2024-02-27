#include "TinyTimber.h"

#ifndef WRITER_H
#define WRITER_H


typedef struct {
    Object super;
} Writer;

#define initWriter() {}

int toggle_port_e(Writer *self, int bit);
int zero_port_e(Writer *self, int bit);

#endif