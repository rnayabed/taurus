

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


void pwmc_gpio(UC channel_no,UC opc)
{
	UC rx = 0;
	UI period = 0;
	UI on_offtime = 0;
	PWMC_Set_Period(PWM_CH_0,period);
	PWMC_Set_OnOffTime(PWM_CH_0,on_offtime);
	PWMC_init(PWM_CH_0,PWM_IDLE_MODE,PWM_LEFT_ALIGN,PWM_LOC_INTR_DIS,opc,PWM_REPEAT_COUNT);

	PWMC_Enable();
}


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

	printf("\n\rPWM AS GPIO");
	printf("\n\r***********");

	printf("\n\rPlease press any key to toggle PWM channel 0\n\n");

	while(1)
	{		
		rx_uart();
		printf("\rHIGH");
		pwmc_gpio(PWM_CH_0, HIGH);
		
		rx_uart();
		printf("\rLOW ");
		pwmc_gpio(PWM_CH_0, LOW);			
	}
}




