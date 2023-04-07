/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 19-Mar-2021
 Filename		: main.c
 Purpose		: To calculate the time between two states
 Description		: To calculate the time between two states
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/

#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"


/** @fn main
 * @brief To calculate the time between two states
 * @details To calculate the time between two states
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main ()
{
	char error;
	clock_t start_time=0, end_time=0, total_time=0;

		while(1)
		{							
			clock_t start_time=0, end_time=0;
			UL total_time=0;
			printf("\nPress any key to start time measurement");
			uart_getchar(UART_0, &error);
			start_time =  get_time();

			
			printf("\nPress any key to stop time measurement");
			uart_getchar(UART_0, &error);

			end_time =  get_time();	
			total_time = (end_time - start_time)*0.025;

			printf("\nTotal time :%10ld micro seconds",total_time);	
			printf("\nTotal time :%10ld seconds\n",total_time/1000000);		

		}
	
}




