

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


void pwmc_continuous_mode(UC channel_no,UC opc,UI period, UI on_offtime)
{
	UC rx = 0;
	PWMC_Set_Period(channel_no,period);
	PWMC_Set_OnOffTime(channel_no,on_offtime);
	PWMC_init(channel_no,PWM_CONTINUOUS_MODE,PWM_LEFT_ALIGN,PWM_LOC_INTR_DIS,opc,PWM_REPEAT_COUNT);

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

	printf("\n\rLED Brightness using PWM");
	printf("\n\r************************* ");

	printf("\n\rConnect LED in PWM0 ");

	while(1)
	{		
		for(int i=0;i<400;i++){
			pwmc_continuous_mode(PWM_CH_0,PWM_OPC_LOW,800000,i*2000);
			udelay(800);			
		}
			
		for(int i=400;i>0;i--){
			pwmc_continuous_mode(PWM_CH_0,PWM_OPC_LOW,800000,i*2000);
			udelay(800);
		}
					
	}
}




