

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


long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*
void pwmc_continuous_mode(UC channel_no,UC opc,UI period, UI on_offtime)
{
	UC rx = 0;
	PWMC_Set_Period(PWM_CH_0,period);
	PWMC_Set_OnOffTime(PWM_CH_0,on_offtime);
	PWMC_init(PWM_CH_0,PWM_CONTINUOUS_MODE,PWM_LEFT_ALIGN,PWM_LOC_INTR_DIS,opc,PWM_REPEAT_COUNT);

	PWMC_Enable();
}
*/

/** @fn main
 * @brief Read all ADC channel values and display it via UART..
 * @details 
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void servoRotate(int pwmPin, int angle)
{
 	long value;

	/*if (angle < 0)
		angle = 0;
	else if (angle > 180)
		angle = 180;
		*/

	value = map(angle, 0, 3000, SERVO_MIN, SERVO_MAX);  	// map the value of pot (ADC value) with the angle to get respective output..

	//printf("value : %d angle : %d\n",value,angle);

	gen_LeftAlign_PWM_Continuous_Mode(pwmPin,value,SERVO_PERIOD);				
			
	udelay(SERVO_DELAY);
	//PWMC_Disable();
}




