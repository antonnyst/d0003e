#include "TinyTimber.h"
#include "writer.h"
#include "pulsegenerator.h"
#include "gui.h"

int main() {

    Writer writer = initWriter();

    PulseGenerator p1 = initPulseGenerator(writer, 4);
    PulseGenerator p2 = initPulseGenerator(writer, 6);

    GUI gui = initGUI(p1, p2);

    INSTALL();
    INSTALL();

    return TINYTIMBER(&gui, init, NULL);
}