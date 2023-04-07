

/***************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  LDR Sensor Demo
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
 @file ldr_sensor_pgm.c
 @brief Main program to check the status of LDR sensor
*/
#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
/** 
 @brief To check the status of LDR sensor
*/
void main()
{
	printf("+-----[ LDR Sensor Demo ]-----+\n");
	printf("|                             |\n");
	printf("|    sensor DO ---> GPIO0     |\n");
	printf("|                             |\n");
	printf("+-----------------------------+\n\n");

	char LDR_Status;
	char prevStatus = 0xff;
	while (1)
	{
		LDR_Status = GPIO_read_pin(0);
		if (prevStatus != LDR_Status)
		{
			prevStatus = LDR_Status; // Saving state.
			if (LDR_Status)
			{
				printf("\rLight Status : Absent  ( )");
			}
			else
			{
				printf("\rLight Status : Present (*)");
			}
		}
	}
}
