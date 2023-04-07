

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
#include "pwmc.h"
#include "stdlib.h"
#include "config.h"
#include "debug_uart.h"

/** @fn pwmc_ch0_intr_handler
 @brief PWMC channel 0 isr
 @details 
 @warning
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void pwmc_ch0_intr_handler(void){

    UI intr_status=PWMCreg(0).Status;
    printf("\n\rIn PWMC Handler:0x ");
    if(intr_status & 0x02) {
		//USER CAN ADD THEIR CODE HERE.
		printf("\n\rPWMC CH0 INTR");
    }
}


/** @fn pwmc_ch1_intr_handler
 @brief PWMC channel 1 isr
 @details 
 @warning
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void pwmc_ch1_intr_handler(void){

    UI intr_status=PWMCreg(1).Status;
    printf("\n\rIn PWMC Handler:0x ");
    if(intr_status & 0x02) {
		//USER CAN ADD THEIR CODE HERE.
		printf("\n\rPWMC CH1 INTR");
    }
}


/** @fn pwmc_ch2_intr_handler
 @brief PWMC channel 2 isr
 @details 
 @warning
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void pwmc_ch2_intr_handler(void){

    UI intr_status=PWMCreg(2).Status;
    printf("\n\rIn PWMC Handler:0x ");
    if(intr_status & 0x02) {
		//USER CAN ADD THEIR CODE HERE.
		printf("\n\rPWMC CH2 INTR");
    }
}


/** @fn pwmc_ch3_intr_handler
 @brief PWMC channel 3 isr
 @details 
 @warning
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void pwmc_ch3_intr_handler(void){

    UI intr_status=PWMCreg(3).Status;
    printf("\n\rIn PWMC Handler:0x ");
    if(intr_status & 0x02) {
		//USER CAN ADD THEIR CODE HERE.
		printf("\n\rPWMC CH3 INTR");
    }
}


/** @fn pwmc_ch4_intr_handler
 @brief PWMC channel 4 isr
 @details 
 @warning
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void pwmc_ch4_intr_handler(void){

    UI intr_status=PWMCreg(4).Status;
    printf("\n\rIn PWMC Handler:0x ");
    if(intr_status & 0x02) {
		//USER CAN ADD THEIR CODE HERE.
		printf("\n\rPWMC CH4 INTR");
    }
}


/** @fn pwmc_ch5_intr_handler
 @brief PWMC channel 5 isr
 @details 
 @warning
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void pwmc_ch5_intr_handler(void){

    UI intr_status=PWMCreg(5).Status;
    printf("\n\rIn PWMC Handler:0x ");
    if(intr_status & 0x02) {
		//USER CAN ADD THEIR CODE HERE.
		printf("\n\rPWMC CH5 INTR");
    }
}


/** @fn pwmc_ch6_intr_handler
 @brief PWMC channel 6 isr
 @details 
 @warning
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void pwmc_ch6_intr_handler(void){

    UI intr_status=PWMCreg(6).Status;
    printf("\n\rIn PWMC Handler:0x ");
    if(intr_status & 0x02) {
		//USER CAN ADD THEIR CODE HERE.
		printf("\n\rPWMC CH6 INTR");
    }
}

/** @fn pwmc_ch7_intr_handler
 @brief PWMC channel 7 isr
 @details 
 @warning
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void pwmc_ch7_intr_handler(void){

    UI intr_status=PWMCreg(7).Status;
    printf("\n\rIn PWMC Handler:0x ");
    if(intr_status & 0x02) {
		//USER CAN ADD THEIR CODE HERE.
		printf("\n\rPWMC CH7 INTR");
    }
}

/** @fn pwmc_channel_one_short_mode_with_intr
 @brief PWMC sample pgm using intr in one short mode.
 @details 
 @warning
 @param[in]  unsigned char opc- 0: Output level is Low in idle mode,1: Output level is High in idle mode,
	     unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7).
 @param[Out] No output parameter.
*/
void pwmc_channel_one_short_mode_with_intr(UC opc,UC channel_no)
{
	UC rx = 0;
	PWMC_Set_Period(channel_no,10);
	PWMC_Set_OnOffTime(channel_no,4);
	PWMC_init(channel_no,PWM_ONE_SHORT_MODE,PWM_RIGHT_ALIGN,PWM_LOC_INTR_EN,opc,PWM_REPEAT_COUNT);

	PWMC_Enable_with_intr();

	while(1){
	}
}


/** @fn main
 * @brief PWMC sample program using interrupt.
 * @details 
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main (void)
{
	UC rx = 0;
	UL selection;

	printf("\n\rPWMC INTR SAMPLE PGM ");
	printf("\n\r*********************** ");
	pwmc_register_isr(PWM_CH_0, pwmc_ch0_intr_handler);
	pwmc_channel_one_short_mode_with_intr(PWM_OPC_HIGH,PWM_CH_0);
	while(1);

}




