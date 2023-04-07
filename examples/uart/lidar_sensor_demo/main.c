/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 24-feb-2021
 Filename		: LiDAR_Sensor_demo.c
 Purpose		: LiDAR Demo
 Description		: LiDAR Demo with uart interface
 Author(s)	   	: thomas sebastian
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/
/**
 @file LiDAR_Sensor_demo.c
 @brief Contains routines for UART interface
 @detail Includes software functions to initialize,
 configure, transmit and receive over UART
 */

#include "uart.h"
#include "stdlib.h"
#include "config.h"
#include "gpio.h"

const int HEADER = 0x59; /*----frame header of data package------------*/
unsigned char uart[9];  /*----save data measured by LiDAR-------------*/
int dist; /*----actual distance measurements of LiDAR---*/      


void Get_Lidar_data(int *distance)
{
	  char error;
	  unsigned char head1 = 0, head2 = 0;
	
	  while (1)
		{
		    head1 = uart_getchar(UART_1, &error);
	            head2 = uart_getchar(UART_1, &error);
			if (head1 == HEADER && head2 == HEADER)
				  break;
		 }
	   for (int i = 2; i < 9; i++)
	    	{
		    uart[i] = uart_getchar(UART_1, &error);
	    	}
	    	
		    uart[0] = HEADER;
		    uart[1] = HEADER;

		    *distance = uart[2] + uart[3] * 256; //the distance
		    //*temperature = ((uart[6] + uart[7] * 32)-256);//the temperature

		    //printf("dist = %d.5 \n", *distance);
		    //printf("temp = %f \n", *temperature);
		    //printf("%d",dist); //output measure distance value of LiDAR
		    //printf("\n");
		   // udelay(10000);
}


void main() 
 { 
 	 uart_set_baud_rate(UART_1,115200,SYSTEM_FREQUENCY);
 
 
 	while(1)
        {     
 		
 		Get_Lidar_data(&dist);
		printf("\rDistance = %5dcm",dist); 
			
			
	
	udelay(5000);
	}
}
 
