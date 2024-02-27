#include "pulsegenerator.h"
#include "writer.h"


// Pulse method
// Toggles the pulse with writer object
int pulse(PulseGenerator *self) {
    
    // Toggle pulse bit
    ASYNC(&(self->writer), toggle_port_e, self->bit);
    
    // Call again if generator is still enabled
    if (self->hz > 0) {
        AFTER(MSEC(1000/(self->hz)), &self, pulse, NULL);
    } else {
        // Generator has been disabled so we set the bit to zero
        ASYNC(&(self->writer), zero_port_e, self->bit);
    }

    return 0;
}

// Increment method
int increment(PulseGenerator *self) {
    if (self->hz == 0) {
        // Starting an disabled generator
        ASYNC(&self, pulse, NULL);
    }
    self->hz = self->hz + 1;
    return 0;
}

// Decrement method
int decrement(PulseGenerator *self) {
    if (self->hz > 0) {
        self->hz = self->hz - 1;
    }
    return 0;
}

// Save/load method
int save_load(PulseGenerator *self) {
    if (self->hz > 0) {
        self->saved = self->hz;
        self->hz = 0;
    } else {
        self->hz = self->saved;
    }

    return 0;
}

// Get_hz method
int get_hz(PulseGenerator *self) {
    return self->hz;
}