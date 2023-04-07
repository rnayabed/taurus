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
 @file ir_code_receive_pgm.c
 @brief Contains routines for read temperature using 1-wire protocol
 */


/** 
 * @brief Proximity sensor program
 * @details Proximity sensor program
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main ()
{
	UC uart_error;	
	US pin_data1 = 0,pin_data4=0, count=0, bit=0;
	UC repeat;	
	UL ir_code[35]={0,};

		repeat = 0;


		printf("\n\r ***************************************************************************");
		printf("\n\r INFO: Connect IR Receive sensor to GPIO 4 ");		
		printf("\n\r ***************************************************************************\n\r\n\r");
		while(1){

		pin_data4 = GPIO_read_pin(4); //SW0 LSB
			if(pin_data4!=1)
				break;
		}

		while(!repeat)
		{	
			//GPIO_write_pin(0,HIGH); //SW0 LSB		
			
			udelay(15); 

			//GPIO_write_pin(0,LOW); //SW0 LSB	
		
			pin_data4 = GPIO_read_pin(4); //SW0 LSB	
					
			ir_code[count] |= (pin_data4 << bit);

			bit++;
			if(bit==32){
				count++;
				bit=0;
			}

			if(count==35)
				break;
		}
		for(int i=0;i<35;i++)
			printf("%08x\n",ir_code[i]);
	
}




