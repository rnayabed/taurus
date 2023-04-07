

/***************************************************
* Module name                           :  main.c
* Name of Author                        :  Premjith A V, C-DAC
* Email ID  (Report any bugs)           :  premjith@cdac.in
* Module Description                    :  Stepper motor Demo
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
 @file stepper_motor_pgm.c
 @brief Main program to rotate the stepper motor
*/


#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

#define HIGH 1
#define LOW  0
/** 
 * @brief Main function to rotate the stepper motor
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main()
{
 int dirPin = 0;
 int stepPin = 1;
 int stepsPerRevolution = 200;
	printf("+--------[ Stepper motor Demo ]-------+\n");
	printf("|                                     |\n");
	printf("|        GPIO0 --->  Direction        |\n");
	printf("|        GPIO1 --->  Steps	      |\n");
	printf("|                                     |\n");
	printf("+-------------------------------------+\n\n");


	while (1)
	{
		  // Set motor direction clockwise
		  GPIO_write_pin(dirPin, HIGH);

		  // Spin motor slowly
		  for(int x = 0; x < stepsPerRevolution; x++)
		  {
		    GPIO_write_pin(stepPin, HIGH);
		    udelay(2000);
		    GPIO_write_pin(stepPin, LOW);
		    udelay(2000);
		  }
		  udelay(100000); // Wait a second
		  
		  // Set motor direction counterclockwise
		  GPIO_write_pin(dirPin, LOW);

		  // Spin motor quickly
		  for(int x = 0; x < stepsPerRevolution; x++)
		  {
		    GPIO_write_pin(stepPin, HIGH);
		    udelay(1000);
		    GPIO_write_pin(stepPin, LOW);
		    udelay(1000);
		  }
		  udelay(100000); // Wait a second
	}
}
