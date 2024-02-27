#include <stdbool.h>
#include "writer.h"

typedef struct {
    int hz;
    int saved;
    int bit;
    bool pulse;
    Writer writer;
} PulseGenerator;

#define initPulseGenerator(writer) {0, 0, writer}

int update(PulseGenerator *self);
int increment(PulseGenerator *self);
int decrement(PulseGenerator *self);
int save_load(PulseGenerator *self);
int get_hz(PulseGenerator *self);
