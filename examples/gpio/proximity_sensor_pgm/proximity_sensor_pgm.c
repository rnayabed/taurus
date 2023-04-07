/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Jul-2020
 Filename		: main.c
 Purpose		: Proximity sensor program
 Description		: Proximity sensor program using gpio
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/

#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

/**
 @file proximity_sensor_pgm.c
 @brief Main program to sense the proximity of object using PIR sensor
*/

/** 
 * @brief Proximity sensor program
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main ()
{
	UC uart_error;	
	US pin_data0 = 0;
	UC repeat,rx = 0;
	
	while(1)
	{
		repeat = 0;


		printf("\n\r ***************************************************************************");
		printf("\n\r INFO: Connect proximity sensor to GPIO 0 ");	
		printf("\n\r INFO: Please try to put an object infront of proximity sensor");	
		printf("\n\r INFO: LED: GREEN , RED will toggle for sensor output");	
		printf("\n\r ***************************************************************************");

		while(!repeat)
		{

			pin_data0 = GPIO_read_pin(0); //SW0 LSB
			
			if((pin_data0 == 1) )
			{					
				GPIO_write_pin(22,ON_LED);
				GPIO_write_pin(23,OFF_LED);				
			}
			else
			{	
				GPIO_write_pin(22,OFF_LED);				
				GPIO_write_pin(23,ON_LED);	
			}
			
		}
	}
	
}




