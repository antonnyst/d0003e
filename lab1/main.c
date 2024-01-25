#include <avr/io.h>


void lcd_init(){

    CLKPR = 0x80;
    CLKPR = 0x00;

    LCDCRB = (1<<LCDCS) | (1<<LCDMUX1) | (1<<LCDMUX0) | (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);
    /* Using 16 as prescaler selection and 8 as LCD Clock Divide /
    LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
    / Set segment drive time to 300 us and output voltage to 3.35 V/
    LCDCCR = (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0);

    / Enable LCD, low power waveform and no interrupt enabled */
    LCDCRA = (1<<LCDEN) | (1<<LCDAB);
}

int main() {
    lcd_init();

    LCDDR0 = 0xf;

    return 0;
}
