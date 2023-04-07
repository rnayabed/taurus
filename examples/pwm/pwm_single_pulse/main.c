

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
#include "pwmc.h"
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
	UC rx = 0;
	UL selection;

	printf("\n\rPWM single pulse generation");
	printf("\n\r*************************** ");

	printf("\n\rGenerate 10us pulse on PWM channel 0 ");
	
	gen_LeftAlign_PWM_OneShort_Mode(PWM_CH_0,400,0,1000);			
	
	while(1);
}




