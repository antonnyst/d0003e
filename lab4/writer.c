#include <avr/io.h> 
#include "writer.h"
#include "lcd.h"

int toggle_port_e(Writer *self, int bit) {
    //int val = (PORTE & (1<<bit)) > 0;
    
    //writeLong(bit);
    
    PINE = 1 << bit;
    
    //PORTE = (PORTE & ~(1<<bit)) | ~(PORTE & (1<<bit));
    
    //writeLong(PORTE);
    
    return 0;
}

int zero_port_e(Writer *self, int bit) {
    PORTE = (PORTE & ~(1<<bit));
}
