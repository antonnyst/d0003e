#include <avr/io.h> 
#include "writer.h"
#include "lcd.h"

int toggle_port_e(Writer *self, int bit) {
    PINE = 1 << bit;    
    return 0;
}

int zero_port_e(Writer *self, int bit) {
    PORTE = (PORTE & ~(1<<bit));
}
