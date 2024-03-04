#include <avr/io.h>
#include "TinyTimber.h"
#include "writer.h"
#include "pulsegenerator.h"
#include "gui.h"
#include "lcd.h"
#include "joystick.h"

// BIT 0 = inget?
// BIT 1 = inget?

// BIT 4 = inget?
// BIT 5 = left = PE6
// BIT 6 = mitten = PE5
// BIT 7 = inget? = ?

int main() {

    Writer writer = initWriter();

    PulseGenerator p1 = initPulseGenerator(5, writer);
    PulseGenerator p2 = initPulseGenerator(6, writer);

    GUI gui = initGUI(p1, p2);

    Joystick joystick = initJoystick(gui);

    INSTALL(&joystick, joystickEventPCINT0, IRQ_PCINT0);
    INSTALL(&joystick, joystickEventPCINT1, IRQ_PCINT1);

    return TINYTIMBER(&joystick, start_joystick, NULL);
}