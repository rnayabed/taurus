

/***************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  Relay Demo
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
 @file relay_pgm.c
 @brief Main program to ON/OF the relay
*/


#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

#define HIGH 1
#define LOW  0
/** 
 * @brief Main function to ON/OFF the relay
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main()
{
	printf("+--------[ Relay Demo ]-------+\n");
	printf("|                             |\n");
	printf("|       GPIO0 --->  Input     |\n");
	printf("|                             |\n");
	printf("+-----------------------------+\n\n");

	char mode = 0xff;
	while (1)
	{
			if (mode)
			{
				printf("\r Relay ON  [ .--. ]");
				GPIO_write_pin(0,HIGH);
			}
			else
			{
				printf("\r Relay OFF [ ./ . ]");
				GPIO_write_pin(0,LOW);
			}
		udelay(400000);
		mode = ~(mode);
	}
}
