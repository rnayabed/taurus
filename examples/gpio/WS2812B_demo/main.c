/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 30-Apr-2021
 See LICENSE for license details.
 ***************************************************/
/**
 @file software_serial.c
 @brief Contains routines for Software based uart
 */

/*  Include section
 ***************************************************/

#include "uart.h"
#include "stdlib.h"
#include "gpio.h"
#include "config.h"
#include "WS2812B.h"

/**
 @fn main
 @brief transmit and reception through uart
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.
 */
union color
{
	struct
	{
		unsigned char blue;
		unsigned char red;
		unsigned char green;
	} RGB;
	int value;
};

unsigned int hash(unsigned int x)
{
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = (x >> 16) ^ x;
	return x;
}
void main()
{
	int transitionDelay = 1000; 
	union color inputColor;
	beginWS2812B(PIN_0, 21);
	printf("\n WS2812B RGB Animation.");
	while (1){
		/* trasition 1  [green 0 -> 255] */
		inputColor.RGB.blue = 0;
		inputColor.RGB.red = 255;
		for (char colorValue = 0; colorValue < 255; colorValue++)
		{
			inputColor.RGB.green = colorValue;
			setWS2812B(inputColor.value);
			udelay(transitionDelay);
		}

		/* trasition 2 [red 255 -> 0] */
		for (char colorValue = 255; colorValue > 0; colorValue--)
		{
			inputColor.RGB.red = colorValue;
			setWS2812B(inputColor.value);
			udelay(transitionDelay);
		}

		/* trasition 3 [blue 0 -> 255] */
		for (char colorValue = 0; colorValue < 255; colorValue++)
		{
			inputColor.RGB.blue = colorValue;
			setWS2812B(inputColor.value);
			udelay(transitionDelay);
		}

		/* trasition 4 [green 255 -> 0] */
		for (char colorValue = 255; colorValue > 0; colorValue--)
		{
			inputColor.RGB.green = colorValue;
			setWS2812B(inputColor.value);
			udelay(transitionDelay);
		}

		/* trasition 5 [red 0 -> 255] */
		for (char colorValue = 0; colorValue < 255; colorValue++)
		{
			inputColor.RGB.red = colorValue;
			setWS2812B(inputColor.value);
			udelay(transitionDelay);
		}

		/* trasition 6 [blue 255 -> 0] */
		for (char colorValue = 255; colorValue > 0; colorValue--)
		{
			inputColor.RGB.blue = colorValue;
			setWS2812B(inputColor.value);
			udelay(transitionDelay);
		}
	}
}
