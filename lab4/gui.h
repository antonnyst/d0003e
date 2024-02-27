#include <stdbool.h>
#include "pulsegenerator.h"

#define GUI_UPDATE_FREQ MSEC(50)

#define initGUI(left, right) {false, left, right}

typedef struct {
    bool active; 
    PulseGenerator left;
    PulseGenerator right;
} GUI;

