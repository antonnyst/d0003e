#include "TinyTimber.h"
#include "writer.h"
#include "pulsegenerator.h"
#include "gui.h"
#include "lcd.h"

int main() {

    Writer writer = initWriter();

    PulseGenerator p1 = initPulseGenerator(4, writer);
    PulseGenerator p2 = initPulseGenerator(6, writer);

    //writeLong(&p1);

    GUI gui = initGUI(p1, p2);

    INSTALL(&gui, joystickEvent, IRQ_PCINT0);
    INSTALL(&gui, joystickEvent, IRQ_PCINT1);

    return TINYTIMBER(&gui, init, NULL);
}