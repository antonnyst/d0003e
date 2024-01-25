#include <avr/io.h>
#include <stdint.h>

#define NIBBLE1 0b1111
#define NIBBLE2 0b1111 << 4
#define NIBBLE3 0b1111 << 8
#define NIBBLE4 0b1111 << 12


void lcd_init(){
    LCDCRB = (1<<LCDCS) | (1<<LCDMUX1) | (1<<LCDMUX0) | (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);
    // Using 16 as prescaler selection and 8 as LCD Clock Divide
    LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
    // Set segment drive time to 300 us and output voltage to 3.35V
    LCDCCR = (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0);
    // Enable LCD, low power waveform and no interrupt enabled
    LCDCRA = (1<<LCDEN) | (1<<LCDAB);
}


//   MPND LEGC JFHB KXSA
// 0b1001 0101 0101 1111


/* struct {
    char '0'
             ABCD EFGH XJKL MNSP
    bits = 0b1101 0100 1001 0010
}*/



void writeRaw(uint32_t data, uint pos) {
    if (pos == 0) {
        LCDDR0  = ((data & NIBBLE1 )       ) | (LCDDR0  & NIBBLE2);
        LCDDR5  = ((data & NIBBLE2 ) >> 4  ) | (LCDDR5  & NIBBLE2);
        LCDDR10 = ((data & NIBBLE3 ) >> 8  ) | (LCDDR10 & NIBBLE2);
        LCDDR15 = ((data & NIBBLE4 ) >> 12 ) | (LCDDR15 & NIBBLE2);
    }
    
    if (pos == 1) {
        LCDDR0  = ((data & NIBBLE1 ) << 4  ) | (LCDDR0  & NIBBLE1);
        LCDDR5  = ((data & NIBBLE2 )       ) | (LCDDR5  & NIBBLE1);
        LCDDR10 = ((data & NIBBLE3 ) >> 4  ) | (LCDDR10 & NIBBLE1);
        LCDDR15 = ((data & NIBBLE4 ) >> 8  ) | (LCDDR15 & NIBBLE1);
    }
}

// 7 = 0b0000000100010001

void writeChar(char ch, uint pos) {
    if (pos > 5) {
        return;
    }

    uint32_t converted = 0b0000000100010001;
    
    writeRaw(converted, pos);
}


int main() {
    CLKPR = 0x80;
    CLKPR = 0x00;

    lcd_init();

    writeChar('0', 0);

    //LCDDR0 = 0b00001111;


    //writeRaw(0b1011, 1);
    //LCDDR0 = (ch && 0xf) || (LCDDR0 && 0xf0);


    return 0;
}
