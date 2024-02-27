#include <stdbool.h>
#include "pulsegenerator.h"


#define initGUI(left, right) {false, left, right}

typedef struct {
    bool active; 
    PulseGenerator left;
    PulseGenerator right;
} GUI;

