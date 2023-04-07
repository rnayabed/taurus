

/***************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  Touch Sensor Demo
*
* Copyright 2020 Company CDAC(T)
* All Rights Reserved.
*
*  The information contained herein is confidential
* property of Company. The user, copying, transfer or
* disclosure of such information is prohibited except
* by express written agreement with Company.
*
***************************************************/

/*  Include section
*
*
***************************************************/
/**
 @file touch_sensor_pgm.c
 @brief Main program for touch sensor application
*/
#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

#define DELAY 7000

void ledDance(){
	/* Red */
	GPIO_write_pin(19,ON_LED);
	GPIO_write_pin(22,ON_LED);
	udelay(DELAY);
	GPIO_write_pin(19,OFF_LED);
	GPIO_write_pin(22,OFF_LED);
	/* Green */
	GPIO_write_pin(20,ON_LED);
	GPIO_write_pin(23,ON_LED);
	udelay(DELAY);
	GPIO_write_pin(20,OFF_LED);
	GPIO_write_pin(23,OFF_LED);
	/* Blue */
	GPIO_write_pin(21,ON_LED);
	GPIO_write_pin(24,ON_LED);
	udelay(DELAY);
	GPIO_write_pin(21,OFF_LED);
	GPIO_write_pin(24,OFF_LED);
}

void main()
{
	printf("+-----[ Touch Sensor Demo ]-----+\n");
	printf("|                               |\n");
	printf("|     sensor SIG ---> GPIO0    |\n");
	printf("|                               |\n");
	printf("+-------------------------------+\n\n");

	char Touch_Status;
	char prevStatus = 0xff;
	while (1)
	{
		Touch_Status = GPIO_read_pin(0);
		if (prevStatus != Touch_Status)
		{
			prevStatus = Touch_Status; // Saving state.
			if (Touch_Status)
			{
				printf("\rStatus :   Touched [ ((.)) ]");
				ledDance();
			}
			else
			{
				printf("\rStatus : Untouched [   .   ]");
				ledDance();
			}
		}
	}
}
