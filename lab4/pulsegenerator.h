#include <stdbool.h>
#include "writer.h"


#ifndef pulsegenerator_H
#define pulsegenerator_H

typedef struct {
    int hz;
    int saved;
    int bit;
    Writer writer;
} PulseGenerator;

#define initPulseGenerator(bit, writer) {0, 0, bit, writer}

int pulse(PulseGenerator *self);
int increment(PulseGenerator *self);
int decrement(PulseGenerator *self);
int save_load(PulseGenerator *self);
int get_hz(PulseGenerator *self);

#endif