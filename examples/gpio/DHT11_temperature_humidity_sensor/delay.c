

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
*
*
***************************************************/

/*  Include section
*
*
***************************************************/

#include "stdlib.h"
#include "config.h"
#include "timer.h"
#include "encoding.h"

#define F_MHZ 40

void sec_delay(UL no_seconds) {
	
	UL cycle_count, current_cycle, target_cycle;
	
	cycle_count = no_seconds * F_MHZ * 1000 * 1000;
	
	current_cycle = read_csr(mcycle);
	target_cycle = current_cycle + cycle_count;
		
	while((read_csr(mcycle) < target_cycle));

}

void millisec_delay(UL no_milli_seconds) {
	
	UL cycle_count, current_cycle, target_cycle;
	
	cycle_count = no_milli_seconds * F_MHZ  * 1000;
	
	current_cycle = read_csr(mcycle);
	target_cycle = current_cycle + cycle_count;
		
	while((read_csr(mcycle) < target_cycle));

}

void microsec_delay(UL no_micro_seconds) {
#if __riscv_xlen == 64
	UL cycle_count, current_cycle, target_cycle;
#else
	UI cycle_count, current_cycle, target_cycle;
#endif
		//no_micro_seconds = no_micro_seconds*0.82;
	cycle_count = no_micro_seconds * F_MHZ;
	
	current_cycle = read_csr(mcycle);
	target_cycle = current_cycle + cycle_count;
		
	while((read_csr(mcycle) < target_cycle));

}
void nanosec_delay(UL no_nano_seconds) {
	
	UL cycle_count, current_cycle, target_cycle;
	
	cycle_count = no_nano_seconds * F_MHZ / 1000;
	
	current_cycle = read_csr(mcycle);
	target_cycle = current_cycle + cycle_count;
		
	while((read_csr(mcycle) < target_cycle));

}
/** @fn main
 * @brief Generate a delay in polling mode.
 * @details In this pgm the timers0,timers1 & timers2 are loaded with values and waits for interrupt in polling mode.
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/





