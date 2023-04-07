
/***************************************************************************
 * Project                               :  MDP
 * Name of the file                      :  timer.c
 * Brief Description of file             :  Driver to control the GPIO.
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
 @file timer.c
 @brief Contains routines for different timer funtions 
 @detail 
 */

#include <include/stdlib.h>
#include <include/timer.h>
#include <include/interrupt.h>
#include <include/config.h>




/** @fn UC timer_put_delay(UC timer_no, UI no_of_clocks)
  @brief  load, enable and wait for interrupt (Polling mode).
  @details The selected timer is loaded with the no of clocks and it is enabled with intr masked. The timer module waits untill it interrupts.

  @param[in] unsigned char, unsigned int
  @param[Out] Returns 1 when interrupt is generated.
*/
UC timer_put_delay(UC timer_no, UI no_of_clocks) {

	UI wEOI;
	volatile UI *TimersRawIntStatusReg = (volatile UI *)(TIMER_BASE_ADDRESS + 0xA8); 	// Global Raw Intr Status Reg

	Timer(timer_no).Control = 0x0;			// Disable timer.
	__asm__ __volatile__ ("fence");
	Timer(timer_no).LoadCount = no_of_clocks;	// Load timer with no of clocks.
	__asm__ __volatile__ ("fence");
	Timer(timer_no).Control = 0x07;			// Enable timer with intr masked
	__asm__ __volatile__ ("fence");
	//while(Timer(timer_no).IntrStatus != 1); 	// Wait for interrupt status bit to set.
	//wEOI = Timer(timer_no).EOI;			// Reads the EOI register to clear the intr.

	if(timer_no == 0){
		while((*TimersRawIntStatusReg & 1) != 1);	// Wait till TIMER0 interrupts.
		Timer(timer_no).Control = 0x0;			// Disable timer.
	}
	if(timer_no == 1){
		while((*TimersRawIntStatusReg & 2) != 2);	// Wait till TIMER1 interrupts.
		Timer(timer_no).Control = 0x0;			// Disable timer.
	}
	if(timer_no == 2){
		while((*TimersRawIntStatusReg & 4) != 4);	// Wait till TIMER2 interrupts.
		Timer(timer_no).Control = 0x0;			// Disable timer.
	}
	return 1;
}



/** @fn UI timer_get_current_value(UC timer_no)
  @brief  Get the current timer value.
  @details The current value of timer is returned.

  @param[in] unsigned char
  @param[Out] The current value of timer.
*/
UI timer_get_current_value(UC timer_no) {

	UI current_val = 0;	
	current_val = Timer(timer_no).CurrentValue; // Get the current value of timer.
	return current_val;   
}



/** @fn void timer_run_in_intr_mode(UC timer_no, UI no_of_clocks)
  @brief  Enable timer interrupt in unmasked & user defined mode..
  @details The selected timer's interrupt is enabled. 
  The LoadCount will  be reloaded with no_of_clocks once the timer is expired and if timer is not disabled.

  @param[in] unsigned char timer_no: Selected timer, unsigned int no_of_clocks : value to be loaded to LoadCount register
  @param[Out] No output parameter.
*/
void timer_run_in_intr_mode(UC timer_no, UI no_of_clocks) {

	Timer(timer_no).Control = 0x0;			// Disable timer.
	__asm__ __volatile__ ("fence");

	Timer(timer_no).LoadCount = no_of_clocks;	// Load timer with no of clocks.
	__asm__ __volatile__ ("fence");

	Timer(timer_no).Control = 0x03;			// Enable timer with intr unmasked.
	__asm__ __volatile__ ("fence");
}

/** @fn void timer_disable(UC timer_no)
  @brief  Disable timer  
  @details The selected timer and its interrupt is disabled.

  @param[in] unsigned char timer_no: Selected timer.
  @param[Out] No output parameter.
*/
void timer_disable(UC timer_no) {

	Timer(timer_no).Control = 0x0;			// Disable timer.
	__asm__ __volatile__ ("fence");	
}

/** @fn void timer_enable(UC timer_no)
  @brief  Enable timer with interrupt 
  @details The selected timer and its interrupt is enabled and unmasked.

  @param[in] unsigned char timer_no: Selected timer.
  @param[Out] No output parameter.
*/
void timer_enable(UC timer_no) {

	Timer(timer_no).Control = 0x03;			// Enable timer with intr unmasked.
	__asm__ __volatile__ ("fence");	
}

/** @fn void timer_register_isr(UC timer_no, void (*timer_isr)())
  @brief  Enable timer interrupt in interrupt controller module.
  @details Invoke the selected timer ISR.

  @param[in] unsigned char timer_no: Selected timer, function pointer to ISR.
  @param[Out] No output parameter.
*/
void timer_register_isr(UC timer_no, void (*timer_isr)()){ ///*timer_isr is function pointer to user defined intr handler

    UC irq_no;
#if __riscv_xlen == 64
	if(timer_no == 0)
		irq_no = 10;
	else if(timer_no == 1)
		irq_no = 11;
	else if(timer_no == 2)
		irq_no = 12;

#else
	if(timer_no == 0)
		irq_no = 7;
	else if(timer_no == 1)
		irq_no = 8;
	else if(timer_no == 2)
		irq_no = 9;
#endif
     	interrupt_enable(irq_no);		//Enable interrupt in controller.
    	irq_register_handler(irq_no, timer_isr); 
}



