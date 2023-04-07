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
#include "debug_uart.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include "pwmc.h"

void pwmc_continuous_mode(UC channel_no,UC opc,UI period, UI on_offtime)
{
	UC rx = 0;
	PWMC_Set_Period(PWM_CH_0,period);
	PWMC_Set_OnOffTime(PWM_CH_0,on_offtime);
	PWMC_init(PWM_CH_0,PWM_CONTINUOUS_MODE,PWM_LEFT_ALIGN,PWM_LOC_INTR_DIS,opc,PWM_REPEAT_COUNT);

	PWMC_Enable();
}


/** @fn main
 * @brief Proximity sensor program
 * @details Proximity sensor program
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void ir_send (int toggle)
{
	UC uart_error;	
	US pin_data1 = 0,pin_data4=0, count=0, bit=0;
	UC repeat;	
	UL ir_code_off[35]={0,}, ir_code_on[35]={0,};
//Code ON Bluestar AC
ir_code_off[0]  = 0xfe000000;                                                                        
ir_code_off[1]  = 0xcff3ffff;                                                                        
ir_code_off[2]  = 0x63fce7f1;                                                                        
ir_code_off[3]  = 0x9fc7f9fe;                                                                        
ir_code_off[4]  = 0xc7f1fc7f;                                                                        
ir_code_off[5]  = 0x318fe31f;                                                                        
ir_code_off[6]  = 0x8c63f8fe;                                                                        
ir_code_off[7]  = 0xe7f9fe7f;                                                                        
ir_code_off[8]  = 0xf1fe73fc;                                                                        
ir_code_off[9]  = 0xfe7f9ce7;                                                                        
ir_code_off[10] = 0x7f1fc7f1;                                                                        
ir_code_off[11] = 0x1fc7f1fc;                                                                        
ir_code_off[12] = 0xc7f1fc7f;                                                                        
ir_code_off[13] = 0xf1fc7f1f;                                                                        
ir_code_off[14] = 0xfcff3fcf;                                                                        
ir_code_off[15] = 0xff3fcff3;                                                                        
ir_code_off[16] = 0x3fcff3fc;                                                                        
ir_code_off[17] = 0x8fe3f8fe;                                                                        
ir_code_off[18] = 0xe3f8fe3f;                                                                        
ir_code_off[19] = 0x18fe3f8f;                                                                        
ir_code_off[20] = 0xfe3f8c63;                                                                        
ir_code_off[21] = 0xf39fe7f9;                                                                        
ir_code_off[22] = 0xfcff3fcf;                                                                        
ir_code_off[23] = 0xff3fcff3;                                                                        
ir_code_off[24] = 0xf18c63fc;                                                                        
ir_code_off[25] = 0x8fe3f8c7;                                                                        
ir_code_off[26] = 0x7f1fc7f1;                                                                        
ir_code_off[27] = 0x1fc7f1fc;                                                                        
ir_code_off[28] = 0xcff39cff;                                                                        
ir_code_off[29] = 0x7f9cff3f;                                                                        
ir_code_off[30] = 0x000001fe;                                                                        
ir_code_off[31] = 0xfffffffc;







//Code OFF Bluestar AC
ir_code_on[0]  = 0xfe000000;
ir_code_on[1]  = 0x8fe3ffff;
ir_code_on[2]  = 0x63f8c7f1;
ir_code_on[3]  = 0x9fc7f1fe;
ir_code_on[4]  = 0xc7f1fc7f;
ir_code_on[5]  = 0x3f8fe31f;
ir_code_on[6]  = 0x9c7f1fc6;
ir_code_on[7]  = 0xff3fcff3;
ir_code_on[8]  = 0x3f8c7f9c;
ir_code_on[9]  = 0x8ff31cfe;
ir_code_on[10] = 0xe3fcfe3f;
ir_code_on[11] = 0xf8fe3f8f;
ir_code_on[12] = 0xfe3f8fe3;
ir_code_on[13] = 0x7f8fe3f8;
ir_code_on[14] = 0x9fe7f9fe;
ir_code_on[15] = 0xc7f9fe7f;
ir_code_on[16] = 0xf1fe7f1f;
ir_code_on[17] = 0xfc7f1fe7;
ir_code_on[18] = 0x7f1fc7f1;
ir_code_on[19] = 0x1fc7f1fc;
ir_code_on[20] = 0xcff18c63;
ir_code_on[21] = 0x73fcff3f;
ir_code_on[22] = 0x9fc7f9fe;
ir_code_on[23] = 0xc7f9fe7f;
ir_code_on[24] = 0x31ce7f1f;
ir_code_on[25] = 0x8c7f18fe;
ir_code_on[26] = 0xe3f8fe3f;
ir_code_on[27] = 0xf9fe3f8f;
ir_code_on[28] = 0xfe739fe7;
ir_code_on[29] = 0xe31fe7f9;
ir_code_on[30] = 0x00003fcf;
ir_code_on[31] = 0xffffff80;
                   





		repeat = 0;
		count = 0;		

		if(toggle== 0){
			while(!repeat)
			{	
				//GPIO_write_pin(0,HIGH); //SW0 LSB
				pin_data4 = ((ir_code_off[count] >> bit) & 0x1);		
				
				udelay(15); 

				//GPIO_write_pin(4,pin_data4); //SW0 LSB
				if(pin_data4)
					PWMC_Disable();	
				else
					pwmc_continuous_mode(PWM_CH_0,PWM_OPC_LOW,1000,500);
							
					
				bit++;
				if(bit==32){
					count++;
					bit=0;
				}

				if(count==32)
					break;
			}
			PWMC_Disable();	
			
				printf("OFF\n");
		}
		else
		{
			while(!repeat)
			{	
				//GPIO_write_pin(0,HIGH); //SW0 LSB
				pin_data4 = ((ir_code_on[count] >> bit) & 0x1);		
				
				udelay(15); 

				//GPIO_write_pin(4,pin_data4); //SW0 LSB
				if(pin_data4)
					PWMC_Disable();	
				else
					pwmc_continuous_mode(PWM_CH_0,PWM_OPC_LOW,1000,500);
							
					
				bit++;
				if(bit==32){
					count++;
					bit=0;
				}

				if(count==32)
					break;
			}
			PWMC_Disable();	
			
				printf("ON\n");
		}	
	
	
}




