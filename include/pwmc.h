#ifndef _PWMC_H
#define _PWMC_H
/**
 @file pwmc.h
 @brief header file for PWMC driver
 @detail 
 */

/*  Include section
* 
*
***************************************************/

#include "stdlib.h"	//for datatypes
#include "config.h"	//for base address

#define LOW			0
#define HIGH			1

#define PWM_CH_0			0
#define PWM_CH_1			1
#define PWM_CH_2			2
#define PWM_CH_3			3
#define PWM_CH_4			4
#define PWM_CH_5			5
#define PWM_CH_6			6
#define PWM_CH_7			7

#define REPEAT_COUNT		5

#define PWM_IDLE_MODE				(0)
#define PWM_ONE_SHORT_MODE			(1)
#define PWM_CONTINUOUS_MODE			(2)

#define PWM_LEFT_ALIGN				(0)
#define PWM_RIGHT_ALIGN				(1)

#define PWM_LOC_INTR_DIS			(0)
#define PWM_LOC_INTR_EN				(1)

#define PWM_OPC_LOW					(0)
#define PWM_OPC_HIGH				(1)

#define PWM_REPEAT_COUNT			(REPEAT_COUNT)

#define PWM_GPE						(1<<0)
#define PWM_GIE						(1<<1)



typedef struct
{

	union
	{
		struct
		{
			UI 	Mode 		: 2,
				AC 		: 2,
				IE 		: 1,
				OPC 		: 1,
				RepeatCount	: 16,
				Rsvd  		: 10;
		}__attribute__((packed)) Bits;
		UI word;
	}Control; 					//0x00
	UI Status; 					//0x04
	UI Period;					//0x08
	UI On_Off;					//0x0c
}PWMCregType;



typedef union
{
	struct
	{
		UI 	Mode 		: 2,
			AC 		: 2,
			IE 		: 1,
			OPC 		: 1,
			RepeatCount	: 16,
			Rsvd  		: 10;
	}Bits;
	UI Value;
}PWMcntrlRegType;


#define PWMC_GCR_REG		(PWMC_BASE_ADDR + 0x80)
#define PWMCreg(n) (*((volatile PWMCregType *)(PWMC_BASE_ADDR +  (n * 0x10))))


/*  Function declaration section
* 
*
*
***************************************************/
void PWMC_Set_Period(UC channel_no, UI period);
void PWMC_Set_OnOffTime(UC channel_no, UI time);
void PWMC_init(UC channel_no,UC mode,UC align,UC intr_en_dis,UC opc, US repeatCount);
void PWMC_Set_mode(UC channel_no,UC mode);
void PWMC_Set_alignment(UC channel_no,UC align);
void PWMC_Set_RepeatCount(UC channel_no,US repeatCount);
void PWMC_Change_mode(void);
void PWMC_Enable(void);
void PWMC_Enable_with_intr(void);
void PWMC_Disable(void);
void pwmc_register_isr(UC pwmc_ch_number,void (*pwmc_isr)());
void pwmc_ch0_intr_handler(void);
void pwmc_ch1_intr_handler(void);
void pwmc_ch2_intr_handler(void);
void pwmc_ch3_intr_handler(void);
void pwmc_ch4_intr_handler(void);
void pwmc_ch5_intr_handler(void);
void pwmc_ch6_intr_handler(void);
void pwmc_ch7_intr_handler(void);
void gen_LeftAlign_PWM_Continuous_Mode(UC channel_no,UI on_time,UI period);
void gen_RightAlign_PWM_Continuous_Mode(UC channel_no,UI on_time,UI period);
void gen_LeftAlign_PWM_OneShort_Mode(UC channel_no,UI on_time,US repeatCount,UI period);
void gen_RightAlign_PWM_OneShort_Mode(UC channel_no,UI off_time,US repeatCount,UI period);
UC pwm_idle_check_opc(UC channel_no);
#endif	/* _PWMC_H */	





