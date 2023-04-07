

/***************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  Speed Sensor Demo
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
 @file speed_sensor_pgm.c
 @brief Main program to measure the speed of the rotating object
*/

#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include "encoding.h"

#define GROOVE_DIV 			20.0
#define RPM_UPDATE_DELAY 	90000
#define USEC_TO_MIN 		60000000
#define GPIO_0 				0
/** 
 * @fn double getRPM(int gpioNum) 
 * @brief To measure the RPM of rotating object
 * @details 
 * @param[in] int gpioNum
 * @param[Out] No output parameter 
*/
double getRPM(int gpioNum) {
	double RPM, totalTime;
	clock_t startTime, endTime;

	while (GPIO_read_pin(gpioNum) == LOW); // For data stabilization.
	while (GPIO_read_pin(gpioNum) == HIGH);

	/* Reading Time from one rising edge to next.*/
	while (GPIO_read_pin(gpioNum) == LOW); 	// Waits till a change occur.
	startTime = read_csr(mcycle);
	while (GPIO_read_pin(gpioNum) == HIGH); // Waits till state is back.
	while (GPIO_read_pin(gpioNum) == LOW);  // Waits till state is back.
	endTime = read_csr(mcycle);

	totalTime = ((endTime - startTime)*0.025);
	RPM = ( 1 / (((double)(totalTime*(GROOVE_DIV))) / USEC_TO_MIN));
   	return RPM;
}
/** 
 * @brief Main program to measure the RPM of rotating object
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main()
{
	printf("+-----[ Speed Sensor Demo ]-----+\n");
	printf("|                               |\n");
	printf("|     sensor DO ----> GPIO0     |\n");
	printf("|                               |\n");
	printf("+-------------------------------+\n\n");

	while (1)
	{
		printf("\r Speed = %6.2f Rotations per Minute ",0);
		printf("\r Speed = %6.2f Rotations per Minute ",getRPM(GPIO_0));
		udelay(RPM_UPDATE_DELAY);
	}
}
