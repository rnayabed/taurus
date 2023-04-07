

/***************************************************
* Module name: main.c
*
* Copyright 2020 Company CDAC(T)
* All Rights Reserved.
*
*  The information contained herein is confidential
* property of Company. The user, copying, transfer or
* disclosure of such information is prohibited except
* by express written agreement with Company.
*
*
* Module Description:
* SPI test pgm
*
***************************************************/

/*  Include section
*
*
***************************************************/

/**
 @file led_toggle_pgm.c
 @brief Main program to toggle the LED's
*/


#include "stdlib.h"
#include "config.h"
#include "led.h"
#include "gpio.h"


/** 
 * @brief Toggle leds LD5,LD6 & LD7.
 * @details Toggle LEDs at finite intervals.
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main ()
{
	US sw_status = 0;
	UI i = 0;
	printf("\n\r Please check the LD5,LD6 and LD7 status on Dev. Board");
	while(1)
	{
		//Turn ON LEDs 5, 6, 7 at GPIO pins 16,17,18 respectively.
		
		
		on_LED(PIN_31);
		//on_LED(PIN_17);
		//on_LED(PIN_18);
		printf("\n\r LEDs ON");
		for(i=0; i< 0x800000;i++); // Delay

		//Turn OFF LEDs 5, 6, 7 at GPIO pins 16,17,18 respectively.

		off_LED(PIN_31);
		//off_LED(PIN_17);
		//off_LED(PIN_18);
		printf("\n\r LEDs OFF");

		for(i=0; i< 0x800000;i++); // Delay

		
	}
	
}




