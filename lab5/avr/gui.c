#include "TinyTimber.h"
#include "gui.h"
#include "lcd.h"
#include "controller.h"

#define GUI_UPDATE_FREQ MSEC(100)

int start_gui(GUI *self) {
    lcd_init();

    ASYNC(self, update, 0);
    ASYNC(&(self->controller), start_controller, NULL);
    return 0;
}

int update(GUI *self) {
    
    int north = SYNC(&(self->controller), get_north_amt, NULL);
    int bridge = SYNC(&(self->controller), get_bridge_amt, NULL);
    int south = SYNC(&(self->controller), get_south_amt, NULL);

    printAt(north, 0);
    printAt(bridge, 2);
    printAt(south, 4);

    AFTER(CURRENT_OFFSET() + GUI_UPDATE_FREQ, self, update_gui, 0);
    return 0;
}