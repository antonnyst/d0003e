#include <avr/io.h> 
#include "writer.h"

int toggle_port_e(Writer *self, int bit) {
    int val = (PORTE & (1<<bit)) > 0;
    PORTE = (PORTE & ~(1<<bit)) | (val << bit);
    return 0;
}

int zero_port_e(Writer *self, int bit) {
    PORTE = (PORTE & ~(1<<bit));
}
