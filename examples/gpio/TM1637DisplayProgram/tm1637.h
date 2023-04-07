#ifndef TM1637_H_
#define TM1637_H_

#include "stdlib.h"
#include "config.h"

#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

#define ANIMATION_DELAY  	35000

typedef struct tm1637Display{
	unsigned char clkPin;
	unsigned char  DioPin;
	unsigned char brightness;
}tm1637;

void setWireInput(unsigned char gpioPin);
void setWireOutput(unsigned char gpioPin);
void bitDelay(void);
unsigned short writeByte(uint8_t data, tm1637 display);
void startTM1637(tm1637 display);
void stopTM1637(tm1637 display);
void clearTM1637(tm1637 display);
void setSegments(const uint8_t segments[], tm1637 display);
void displayString(tm1637 display, uint8_t *string, int length);
void setDots(uint8_t *string);

#endif /*TM1637_H_*/
