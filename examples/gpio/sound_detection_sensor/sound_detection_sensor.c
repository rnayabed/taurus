/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Jul-2020
 Filename		: main.c
 Purpose		: Proximity sensor program
 Description		: pir motion sensor program using gpio
 Author(s)		: Karthika P
 Email			: karthikap@cdac.in
    
 See LICENSE for license details.
******************************************************************************/

#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

/**
 @file sound_detection_sensor.c
 @brief Main program to detect the sound
*/



/** 
 * @brief Main program to detect the sound
 * @details pir motion sensor program
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main ()
{

	US pin_data0 = 0;
	UC repeat,rx = 0;
	
	while(1)
	{
		repeat = 0;


		printf("\n\r ***************************************************************************");
		printf("\n\r INFO: Connect PIR to GPIO 0 ");
		printf("\n\r INFO: Trying to get some sound");

		printf("\n\r ***************************************************************************");

		while(!repeat)
		{
			//rx=0;
			pin_data0 = GPIO_read_pin(0); //SW0 LSB
			
			if((pin_data0 == 1) )
			{
				if (rx==0)		{
				printf("\n\r Hurry! we have some sound");
				rx=1;
			}
			}

			else
			{	

			if(rx==1){
				//printf("\n\r no sound");
				rx=0;
			}
			
		}

	}
	
}


}

