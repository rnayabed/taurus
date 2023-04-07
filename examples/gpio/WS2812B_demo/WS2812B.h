#ifndef WS2812B_H_
#define WS2812B_H_

#include "stdlib.h"
#include "config.h"

#define WS2812B_COLOR_BITS 24
#define PIN_DATA_0 0x0
#define PIN_DATA_1 0xFFFF

/* Functions for Thejas32 1-wire GPIO */

void beginWS2812B(unsigned int gpioPin, unsigned int LEDs);
void setWS2812B(int colorData);

#endif /*SOFT_UART_H_*/
