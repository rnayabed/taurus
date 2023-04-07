/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 24-feb-2020
 Filename		: main.c
 Purpose		: I2C & MAX30102 demo
 Description	: Sample MAX30102 with I2C interface
 Author(s)		: 
 Email			: 

 See LICENSE for license details.
 ***************************************************/

/**
 @file main.c
 */

#include "stdlib.h"
#include "MAX30100.h"
#include "gpio.h"
#include "algorithm.h"





void main() 
{
	uint32_t aun_ir_buffer[100]; //infrared LED sensor data
	uint32_t aun_red_buffer[100];  //red LED sensor data
	int32_t n_ir_buffer_length; //data length
	int32_t n_spo2;  //SPO2 value
	int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
	int32_t n_heart_rate; //heart rate value
	int8_t  ch_hr_valid;
	
	
	UC reg_config, intr_status;
	
	MAX30100_reset();
	
	reg_config = readRegister(REG_MODE_CONFIG);
	

	MAX30100_init();
	
	reg_config = readRegister(REG_MODE_CONFIG);
	
	while (1) 
	{		
	 	uint32_t un_min, un_max, un_prev_data, un_brightness;  //variables to calculate the on-board LED brightness that reflects the heartbeats
		int32_t i;
		float f_temp;
		un_brightness=0;
		un_min=0x3FFFF;
		un_max=0;
		
		n_ir_buffer_length=100;  //buffer length of 100 stores 4 seconds of samples running at 25sps
		
		
		//read the first 100 samples, and determine the signal range
		for(i=0;i<n_ir_buffer_length;i++)
		{
			MAX30100_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));  //read from MAX30102 FIFO
			if(un_min>aun_red_buffer[i])
			un_min=aun_red_buffer[i];  //update signal min
			if(un_max<aun_red_buffer[i])
			un_max=aun_red_buffer[i];  //update signal max
			// printf("\nred = %d \t", aun_red_buffer[i]);
			 //printf("ir= %d", aun_ir_buffer[i]);
		}
		un_prev_data=aun_red_buffer[i];
		//udelay(200000);
		
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
			
					printf("\rred = %d \t", aun_red_buffer[i]);
			 		printf("ir= %d\t", aun_ir_buffer[i]);
			 		
			
		if((aun_red_buffer[i] >10000  && aun_ir_buffer[i] > 10000)&&(n_heart_rate > 0 && n_heart_rate < 150 && n_spo2 > 0 && n_spo2 < 100))
			{
					
				printf( "heart_rate=%d\t,spo2=%d", n_heart_rate, n_spo2); //print heart rate and spo2 value. 
					
			}
			
					
			
		udelay(2000);
	
	}
}

	

