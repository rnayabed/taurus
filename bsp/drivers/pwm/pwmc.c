/***************************************************************************
 * Project                               :  MDP
 * Name of the file                      :  pwmc.c
 * Brief Description of file             :  Driver for PWM controller.
 * Name of Author                        :  Sreeju G R
 * Email ID                              :  sreeju@cdac.in

  Copyright (C) 2020  CDAC(T). All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/
/**
 @file pwmc.c
 @brief Contains routines for different PWM options 
 @detail 
 */

#include <include/stdlib.h>
#include <include/pwmc.h>
#include <include/config.h>
#include <include/interrupt.h>

PWMcntrlRegType gPWMCtransfer;


/** @fn void PWMC_Set_Period(UC channel_no, UI period)
 * @brief PWM cycle duration.
 * @details 

 * @param[in] unsigned char channel_no: The channel number to which device is connected (It can be from 0 to 7),
	      unsigned int period: Minimum value supported is 2. Maximum value supported 2^32 – 1.
 * @param[Out] No output parameter
*/


void PWMC_Set_Period(UC channel_no, UI period)
{
	PWMCreg(channel_no).Period = period;
	__asm__ __volatile__ ("fence");
	return;
}


/** @fn void PWMC_Set_OnOffTime(UC channel_no, UI time)
 * @brief Defined as pulse duration in a PWM cycle
 * @details When Alignment register is set for left alignment this register
	    represents on time or high time of the PWM signal.

	    When Alignment register is set for right alignment this register
	    represents off time or low time of the PWM signal.

 * @param[in] unsigned char channel_no: The channel number to which device is connected (It can be from 0 to 7),
	      unsigned int time: Minimum value supported is 1. Maximum value supported 2^32 – 1.
 * @param[Out] No output parameter
*/


void PWMC_Set_OnOffTime(UC channel_no, UI time)
{
	PWMCreg(channel_no).On_Off = time;
	__asm__ __volatile__ ("fence");
	return;
}


/** @fn void PWMC_init(UC channel_no,UC mode,UC align,UC intr_en_dis,UC opc, US repeatCount)
 * @brief 
 * @details 

 * @param[in] unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7),
	      unsigned char mode- 00 : PWM Idle, 01: One short mode, 10: Continuous mode, 11: Reserved for future use.
	      unsigned char align- 00: Left alignment, 01: Right alignment. 10,11: Reserved for future use.
	      unsigned char intr_en_dis- 0 - Disable interrupt, 1 - Enable interrupt.
	      unsigned char opc- 0: Output level is Low in idle mode,1: Output level is High in idle mode
	      unsigned short  repeatCount- Repeat count for the PWM cycle.
 * @param[Out] No output parameter
*/

void PWMC_init(UC channel_no,UC mode,UC align,UC intr_en_dis,UC opc, US repeatCount)
{
	PWMCreg(channel_no).Control.word = 0;
	gPWMCtransfer.Bits.Mode 	= mode;
	gPWMCtransfer.Bits.AC 		= align;
	gPWMCtransfer.Bits.IE		= intr_en_dis;
	gPWMCtransfer.Bits.OPC		= opc;
	gPWMCtransfer.Bits.RepeatCount	= repeatCount;

	PWMCreg(channel_no).Control.word = gPWMCtransfer.Value;
	__asm__ __volatile__ ("fence");
	return;
}

/** @fn void PWMC_Set_mode(UC channel_no,UC mode)
 * @brief 
 * @details 

 * @param[in] unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7),
	      unsigned char mode- 00 : PWM Idle, 01: One short mode, 10: Continuous mode, 11: Reserved for future use.
 * @param[Out] No output parameter
*/
void PWMC_Set_mode(UC channel_no,UC mode)
{
	gPWMCtransfer.Value = PWMCreg(channel_no).Control.word;
	gPWMCtransfer.Bits.Mode = mode;
	PWMCreg(channel_no).Control.word = gPWMCtransfer.Value;
	__asm__ __volatile__ ("fence");
	return;
}

/** @fn void PWMC_Set_alignment(UC channel_no,UC align)
 * @brief 
 * @details 

 * @param[in] unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7),
	      unsigned char align- 00: Left alignment, 01: Right alignment. 10,11: Reserved for future use.
 * @param[Out] No output parameter
*/
void PWMC_Set_alignment(UC channel_no,UC align)
{
	gPWMCtransfer.Value = PWMCreg(channel_no).Control.word;
	gPWMCtransfer.Bits.AC = align;
	PWMCreg(channel_no).Control.word = gPWMCtransfer.Value;
	__asm__ __volatile__ ("fence");
	return;
}

/** @fn void PWMC_Set_RepeatCount(UC channel_no,US repeatCount)
 * @brief 
 * @details 

 * @param[in] unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7),
	      unsigned short  repeatCount- Repeat count for the PWM cycle.
 * @param[Out] No output parameter
*/
void PWMC_Set_RepeatCount(UC channel_no,US repeatCount)
{
	gPWMCtransfer.Value = PWMCreg(channel_no).Control.word;
	gPWMCtransfer.Bits.RepeatCount	= repeatCount;
	PWMCreg(channel_no).Control.word = gPWMCtransfer.Value;
	__asm__ __volatile__ ("fence");
	return;
}

/** @fn PWMC_Enable
 * @brief  Enale PWM.
 * @details 

 * @param[in] No input parameter
 * @param[Out] No output parameter
*/
void PWMC_Enable(void)
{
	UI *ptrPWM_Global_Ctrl = (UI*)PWMC_GCR_REG;
	*ptrPWM_Global_Ctrl = 0;
	*ptrPWM_Global_Ctrl = PWM_GPE;
	__asm__ __volatile__ ("fence");
	return;
}


/** @fn PWMC_Enable_with_intr
 * @brief 
 * @details 

 * @param[in] No input parameter
 * @param[Out] No output parameter
*/

void PWMC_Enable_with_intr(void)
{
	UI *ptrPWM_Global_Ctrl = (UI*)PWMC_GCR_REG;
	*ptrPWM_Global_Ctrl = 0;
	*ptrPWM_Global_Ctrl = (PWM_GPE|PWM_GIE);
	__asm__ __volatile__ ("fence");
	return;
}



/** @fn void PWMC_Disable(void)
 * @brief  Disable interrupt
 * @details 

 * @param[in] No input parameter
 * @param[Out] No output parameter
*/

void PWMC_Disable(void)
{
	UI *ptrPWM_Global_Ctrl = (UI*)PWMC_GCR_REG;
	*ptrPWM_Global_Ctrl = (0<<0);
	__asm__ __volatile__ ("fence");
	return;
}


/** @fn void gen_LeftAlign_PWM_OneShort_Mode(UC channel_no,UI on_time,US repeatCount,UI period)
 * @brief   Generate a One short mode Left aligned PWM signal in the specified channel number.
 * @details 

 * @param[in] unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7),
	      unsigned int on_time: Minimum value supported is 1. Maximum value supported 2^32 – 1.(represents on time or high time of the PWM signal.)
	      unsigned short  repeatCount- Repeat count for the PWM cycle.
	      unsigned int period: Minimum value supported is 2. Maximum value supported 2^32 – 1.
 * @param[Out] No output parameter
*/

void gen_LeftAlign_PWM_OneShort_Mode(UC channel_no,UI on_time,US repeatCount,UI period)
{

	PWMCreg(channel_no).On_Off = on_time;
	__asm__ __volatile__ ("fence");

	gPWMCtransfer.Bits.Mode 	= PWM_ONE_SHORT_MODE;
	gPWMCtransfer.Bits.AC 		= PWM_LEFT_ALIGN;
	//gPWMCtransfer.Bits.IE		= intr_en_dis;
	//gPWMCtransfer.Bits.OPC	= opc;
	gPWMCtransfer.Bits.RepeatCount	= repeatCount;
	PWMCreg(channel_no).Control.word = gPWMCtransfer.Value;
	__asm__ __volatile__ ("fence");

	PWMCreg(channel_no).Period = period;
	__asm__ __volatile__ ("fence");

	PWMC_Enable();

	return;
}


/** @fn void gen_RightAlign_PWM_OneShort_Mode(UC channel_no,UI off_time,US repeatCount,UI period)
 * @brief   Generate a One short mode Right aligned PWM signal in the specified channel number.
 * @details 

 * @param[in] unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7),
	      unsigned int off_time: Minimum value supported is 1. Maximum value supported 2^32 – 1.(represents off time or low time of the PWM signal.)
	      unsigned short  repeatCount- Repeat count for the PWM cycle.
	      unsigned int period: Minimum value supported is 2. Maximum value supported 2^32 – 1.
 * @param[Out] No output parameter
*/

void gen_RightAlign_PWM_OneShort_Mode(UC channel_no,UI off_time,US repeatCount,UI period)
{

	PWMCreg(channel_no).On_Off = off_time;
	__asm__ __volatile__ ("fence");

	gPWMCtransfer.Bits.Mode 	= PWM_ONE_SHORT_MODE;
	gPWMCtransfer.Bits.AC 		= PWM_RIGHT_ALIGN;
	//gPWMCtransfer.Bits.IE		= intr_en_dis;
	//gPWMCtransfer.Bits.OPC	= opc;
	gPWMCtransfer.Bits.RepeatCount	= repeatCount;
	PWMCreg(channel_no).Control.word = gPWMCtransfer.Value;
	__asm__ __volatile__ ("fence");

	PWMCreg(channel_no).Period = period;
	__asm__ __volatile__ ("fence");

	PWMC_Enable();

	return;
}


/** @fn void gen_LeftAlign_PWM_Continuous_Mode(UC channel_no,UI on_time,UI period)
 * @brief   Generate a Continuous mode Left aligned PWM signal in the specified channel number.
 * @details 

 * @param[in] unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7),
	      unsigned int on_time: Minimum value supported is 1. Maximum value supported 2^32 – 1.(represents on time or high time of the PWM signal.)
	      unsigned int period: Minimum value supported is 2. Maximum value supported 2^32 – 1.
 * @param[Out] No output parameter
*/

void gen_LeftAlign_PWM_Continuous_Mode(UC channel_no,UI on_time,UI period)
{

	PWMCreg(channel_no).On_Off = on_time;
			printf("\rHIGH");
	__asm__ __volatile__ ("fence");

	gPWMCtransfer.Bits.Mode 	= PWM_CONTINUOUS_MODE;
	gPWMCtransfer.Bits.AC 		= PWM_LEFT_ALIGN;
	//gPWMCtransfer.Bits.IE		= intr_en_dis;
	//gPWMCtransfer.Bits.OPC	= opc;
	PWMCreg(channel_no).Control.word = gPWMCtransfer.Value;
	__asm__ __volatile__ ("fence");

	PWMCreg(channel_no).Period = period;
	__asm__ __volatile__ ("fence");

	PWMC_Enable();
	return;
}



/** @fn void gen_RightAlign_PWM_Continuous_Mode(UC channel_no,UI on_time,UI period)
 * @brief   Generate a Continuous mode Right aligned PWM signal in the specified channel number.
 * @details 

 * @param[in] unsigned char channel_no- The channel number to which device is connected (It can be from 0 to 7),
	      unsigned int off_time: Minimum value supported is 1. Maximum value supported 2^32 – 1.(represents off time or low time of the PWM signal.)
	      unsigned int period: Minimum value supported is 2. Maximum value supported 2^32 – 1.
 * @param[Out] No output parameter
*/

void gen_RightAlign_PWM_Continuous_Mode(UC channel_no,UI on_time,UI period)
{

	PWMCreg(channel_no).On_Off = on_time;
	__asm__ __volatile__ ("fence");

	gPWMCtransfer.Bits.Mode 	= PWM_CONTINUOUS_MODE;
	gPWMCtransfer.Bits.AC 		= PWM_RIGHT_ALIGN;
	//gPWMCtransfer.Bits.IE		= intr_en_dis;
	//gPWMCtransfer.Bits.OPC	= opc;
	PWMCreg(channel_no).Control.word = gPWMCtransfer.Value;
	__asm__ __volatile__ ("fence");

	PWMCreg(channel_no).Period = period;
	__asm__ __volatile__ ("fence");

	PWMC_Enable();
	return;
}




/** @fn void pwmc_register_isr(UC pwmc_ch_number,void (*pwmc_isr)())
 * @brief  Register PWMC isr
 * @details 

 * @param[in] unsigned char pwmc_ch_number: The channel number to which device is connected (It can be from 0 to 7),
	      function pointer to the pwmc isr().
 * @param[Out] No output parameter
*/
void pwmc_register_isr(UC pwmc_ch_number,void (*pwmc_isr)())
{
    	UC irq_no;
#if __riscv_xlen == 64
	if(pwmc_ch_number == 0)
		irq_no = 54;
	else if(pwmc_ch_number == 1)
		irq_no = 55;
	else if(pwmc_ch_number == 2)
		irq_no = 56;
	else if(pwmc_ch_number == 3)
		irq_no = 57;
	else if(pwmc_ch_number == 4)
		irq_no = 58;
	else if(pwmc_ch_number == 5)
		irq_no = 59;
	else if(pwmc_ch_number == 6)
		irq_no = 60;
	else if(pwmc_ch_number == 7)
		irq_no = 61;
#else
	if(pwmc_ch_number == 0)
		irq_no = 24;
	else if(pwmc_ch_number == 1)
		irq_no = 25;
	else if(pwmc_ch_number == 2)
		irq_no = 26;
	else if(pwmc_ch_number == 3)
		irq_no = 27;
	else if(pwmc_ch_number == 4)
		irq_no = 28;
	else if(pwmc_ch_number == 5)
		irq_no = 29;
	else if(pwmc_ch_number == 6)
		irq_no = 30;
	else if(pwmc_ch_number == 7)
		irq_no = 31;
#endif
	interrupt_enable(irq_no);		//Enable interrupt in controller.
    	irq_register_handler(irq_no, pwmc_isr);
}

UC pwm_idle_check_opc(UC channel_no){
	 gPWMCtransfer.Value = PWMCreg(channel_no).Control.word;
	if(gPWMCtransfer.Bits.OPC & 0x01 ==0x01)
		return 1;
	else
		return 0;
}

