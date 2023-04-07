

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
//#include "adc.h"
#include "stdlib.h"
#include "config.h"
#include "led.h"
#include "gpio.h"
#include "i2c_api.h"
#include "ads1015.h"

/**
 @file pulse_sensor_demo.c
 @brief Contains routines for Read all ADC channel values and display it via UART
 */

int threshold = 2130; //Please change the threshold according to the sensor

/** 
 * @brief Read all ADC channel values and display it via UART..
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main(void)
{
	US adc_data = 0;
	int count = 0, flag = 1;
	clock_t start_time = 0, end_time = 0;
	UL total_time = 0;
	ads1015Begin(I2C_ADC);

	printf("\n\r Reading Pulse\n\r");

	start_time = get_time();
	 printf("\n time 1:%d \n ", start_time);
	while (1)
	{

		adc_data = ads1015ReadSingleEnded(1); //A1
		//result_volt = (adc_data * volt_per_division);
		printf ("%d\n", adc_data);
		end_time = get_time();
         printf("\n time 2:%d \n ", end_time);
		total_time = (((end_time - start_time) * 0.025) / 1000000);

        printf("\n time 3:%d  ", total_time);


		while (total_time <= 10)
		{

            printf(" \r time 4  ");

			adc_data = ads1015ReadSingleEnded(1); //A1

			if (adc_data > threshold)
			{
				if (flag == 1)
				{
					count++;
				}
				on_LED(PIN_23); //RED
				flag = 0;
			}

			else
			{
				flag = 1;
				off_LED(PIN_23); //RED
			}
			udelay(2000);

			adc_data = ads1015ReadSingleEnded(1); //A0

			end_time = get_time();
			total_time = (((end_time - start_time) * 0.025) / 1000000);
		}
		printf("\n count:%d  ", count);

		printf("\n BPM :%d", count * 6); // BPM
		return;
	}
}
