/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 20-july-2021
 Filename		: MQ3_gas_sensor_demo.c
 Purpose		:  UART communication with MQ3 gas sensor
 Description	: Alchohol detection
 Author(s)		: Thomas Sebastian, C-DAC
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/

//#include "adc.h"
#include "stdlib.h"
#include "config.h"
#include "led.h"
#include "gpio.h"
#include "ads1015.h"
#include "i2c_api.h"

/**
 @file MQ3_gas_sensor_demo.c
 @brief Contains routines for Read all ADC channel values and display it via UART
 */

int threshold = 375; //Please change the threshold according to the sensor

/** 
 * @brief Read all ADC channel values and display it via UART..
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main(void)
{
	
	US adc_data = 0;

	printf("\r Connect A0 of MQ3 to A1 pin in VEGA board & adjust threshold ");

	printf("\n\r Reading gas sensor\n\r");

	ads1015Begin(I2C_ADC);
	while (1)
	{
		adc_data = ads1015ReadSingleEnded(1); //A1

		printf("value = %d\n",adc_data);

		if (adc_data >= threshold)
		{
			printf("Alcohol detected!!\n");
			off_LED(PIN_24); //RED
			on_LED(PIN_22); //GREEN
		}
		else
		{
			printf("Alcohol Free!!\n");
			
			on_LED(PIN_24); //RED
			off_LED(PIN_22); //GREEN
		}
		udelay(1000000);
	}
}

