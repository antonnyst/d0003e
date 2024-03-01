#include <stdbool.h>
#include "writer.h"
#include "TinyTimber.h"

#ifndef pulsegenerator_H
#define pulsegenerator_H

typedef struct {
    Object super;
    int hz;
    int saved;
    int bit;
    Writer writer;
} PulseGenerator;

#define initPulseGenerator(bit, writer) {initObject(), 0, 0, bit, writer}

int pulse(PulseGenerator *self);
int increment(PulseGenerator *self, int *arg);
int decrement(PulseGenerator *self, int *arg);
int save_load(PulseGenerator *self);
int get_hz(PulseGenerator *self);

#endif