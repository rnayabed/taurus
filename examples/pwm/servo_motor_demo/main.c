

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
#include "servo.h"
#include "stdlib.h"
#include "config.h"
#include "debug_uart.h"


/** @fn main
 * @brief Read all ADC channel values and display it via UART..
 * @details 
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main (void)
{
	int pos = 0;


	printf("\n\rSERVO MOTOR SWEEP ");
	printf("\n\r****************** ");

	printf("\n\rConnect motor in PWM CH0 ");


	while(1)
	{
		pos = 0;
		for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
		    // in steps of 1 degree
		   servoRotate(PWM_CH_0,pos);	              // tell servo to go to position in variable 'pos'		                        
		  }
		  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
		  servoRotate(PWM_CH_0,pos);	       // tell servo to go to position in variable 'pos'
		   
		  }
	}

}




