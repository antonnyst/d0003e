#include "tinythreads.h"
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>


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


// Raw thing sent to display
//         MPND LEGC JFHB KXSA
//       0b1001 0101 0101 1111

// We store as     
//         ABCD EFGH XJKL MNSP
//bits = 0b1101 0100 1001 0010

uint16_t segments_to_raw(uint16_t segments) {
    return 
	    ((segments & 0b1000000000000000) >> 15 ) |
	    ((segments & 0b0100000000000000) >> 10 ) |
	    ((segments & 0b0010000000000000) >> 5  ) |
	    ((segments & 0b0001000000000000)       ) |
	    ((segments & 0b0000100000000000) >> 1  ) |
	    ((segments & 0b0000010000000000) >> 4  ) |
	    ((segments & 0b0000001000000000)       ) |
	    ((segments & 0b0000000100000000) >> 3  ) |
	    ((segments & 0b0000000010000000) >> 5  ) |
	    ((segments & 0b0000000001000000) << 1  ) |
	    ((segments & 0b0000000000100000) >> 2  ) |
	    ((segments & 0b0000000000010000) << 7  ) |
	    ((segments & 0b0000000000001000) << 12 ) |
	    ((segments & 0b0000000000000100) << 11 ) |
	    ((segments & 0b0000000000000010)       ) |
	    ((segments & 0b0000000000000001) << 14 );
}

typedef struct {
    char c;
    //         ABCD EFGH XJKL MNSP
    //bits = 0b1101 0100 1001 0010
    uint16_t bits;
} SegmentData;

SegmentData segmentData[10] = {
    {'0', 0b1111110000000000},
    {'1', 0b0110000000000000},
    {'2', 0b1101101000010000},
    {'3', 0b1111001000010000},
    {'4', 0b0110011000010000},
    {'5', 0b1011011000010000},
    {'6', 0b1011111000010000},
    {'7', 0b1110000000000000},
    {'8', 0b1111111000010000},
    {'9', 0b1111011000010000}
};

void writeRaw(uint16_t data, int pos) {
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

    if (pos == 2) {
        LCDDR1  = ((data & NIBBLE1 )       ) | (LCDDR1 & NIBBLE2);
        LCDDR6  = ((data & NIBBLE2 ) >> 4  ) | (LCDDR6  & NIBBLE2);
        LCDDR11 = ((data & NIBBLE3 ) >> 8  ) | (LCDDR11 & NIBBLE2);
        LCDDR16 = ((data & NIBBLE4 ) >> 12 ) | (LCDDR16 & NIBBLE2);
    }
    
    if (pos == 3) {
        LCDDR1  = ((data & NIBBLE1 ) << 4  ) | (LCDDR1  & NIBBLE1);
        LCDDR6  = ((data & NIBBLE2 )       ) | (LCDDR6  & NIBBLE1);
        LCDDR11 = ((data & NIBBLE3 ) >> 4  ) | (LCDDR11 & NIBBLE1);
        LCDDR16 = ((data & NIBBLE4 ) >> 8  ) | (LCDDR16 & NIBBLE1);
    }

    if (pos == 4) {
        LCDDR2  = ((data & NIBBLE1 )       ) | (LCDDR2  & NIBBLE2);
        LCDDR7  = ((data & NIBBLE2 ) >> 4  ) | (LCDDR7  & NIBBLE2);
        LCDDR12 = ((data & NIBBLE3 ) >> 8  ) | (LCDDR12 & NIBBLE2);
        LCDDR17 = ((data & NIBBLE4 ) >> 12 ) | (LCDDR17 & NIBBLE2);
    }
    
    if (pos == 5) {
        LCDDR2  = ((data & NIBBLE1 ) << 4  ) | (LCDDR2  & NIBBLE1);
        LCDDR7  = ((data & NIBBLE2 )       ) | (LCDDR7  & NIBBLE1);
        LCDDR12 = ((data & NIBBLE3 ) >> 4  ) | (LCDDR12 & NIBBLE1);
        LCDDR17 = ((data & NIBBLE4 ) >> 8  ) | (LCDDR17 & NIBBLE1);
    }
}

// 7 = 0b0000000100010001

uint16_t get_segments(char ch) {
    for(int i = 0; i < 10; i++) {
        if (segmentData[i].c == ch) {
            return segmentData[i].bits;
        }
    }
    return 0;
}

void writeChar(char ch, int pos) {
    if (pos > 5) {
        return;
    }

    uint16_t bits = get_segments(ch);

    uint16_t converted = segments_to_raw(bits);
    
    writeRaw(converted, pos);
}

bool is_prime(long i) {
    for(long n = 2; n < i; n++) {
        if(i % n == 0) {
            return false;
        }
    }
    return true;
}

int pp;
mutex ppm = MUTEX_INIT;

void printAt(long num, int pos) {
    //lock(&ppm);
    pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    for(volatile int i = 0; i < 20000; i++) {}
    pp++;
    writeChar( num % 10 + '0', pp);
    //unlock(&ppm);
}

void computePrimes(int pos) {
    long n;

    for(n = 1; ; n++) {
        if (is_prime(n)) {
            printAt(n, pos);
            //yield();
        }
    }
}

int main() {
    CLKPR = 0x80;
    CLKPR = 0x00;

    lcd_init();
    spawn(computePrimes, 0);
    computePrimes(3);
}
