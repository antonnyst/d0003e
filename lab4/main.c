#include "TinyTimber.h"
#include "writer.h"
#include "pulsegenerator.h"
#include "gui.h"
#include "lcd.h"

// BIT 4 = inget?
// BIT 5 = left = PE6
// BIT 6 = mitten = PE5
// BIT 7 = inget? = ?

int main() {

    Writer writer = initWriter();

    PulseGenerator p1 = initPulseGenerator(5, writer);
    PulseGenerator p2 = initPulseGenerator(6, writer);

    GUI gui = initGUI(p1, p2);

    INSTALL(&gui, joystickEvent, IRQ_PCINT0);
    INSTALL(&gui, joystickEvent, IRQ_PCINT1);

    return TINYTIMBER(&gui, init, NULL);
}