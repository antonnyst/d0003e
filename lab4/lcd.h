#ifndef LCD_H
#define LCD_H

void lcd_init();
void writeChar(char ch, int pos);
void writeLong(long i);
void printAt(long num, int pos);

#endif