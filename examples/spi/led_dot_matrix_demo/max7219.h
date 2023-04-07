#ifndef _max7219_H
#define max7219_H

#define HIGH 1
#define LOW 0

#define DISPLAY_TEST 0xFF
#define FULL_BRIGHT 0x1
#define FULL_DIM 0x0
#define DIGIT_O 0x1
#define DIGIT_1 0x2
#define DIGIT_2 0x3
#define DIGIT_3 0x4
#define DIGIT_4 0x5
#define DIGIT_5 0x6
#define DIGIT_6 0x7
#define DIGIT_7 0x8
#define INTENSITY 0xA

#define DECODE_MODE 0x9
#define SCANLIMIT   11
#define SHUTDWN   12

void max7219Begin(unsigned char spiNo);
void max7219_test();
void max7219_Brightness(short writeData);
void max7219Write(short writeData);
void max7219PrintScreen(char *writeBuff);
void max7219ClearScreen();
void max7219ScrollText(char * textBuff, int delay, int length);

#endif /*_max7219_*/
