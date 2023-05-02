// SPDX-License-Identifier: MIT

/**
 @file timer.c
 @brief Contains routines for different timer funtions 
 @detail 
 */

#include <stdlib.h>
#include <timer.h>
#include <interrupt.h>
#include <config.h>



/** @fn unsigned char timer_put_delay(unsigned short timer, unsigned long clocks)
  @brief  load, enable and wait for interrupt (Polling mode).
  @details The selected timer is loaded with the no of clocks and it is enabled with intr masked. The timer module waits untill it interrupts.

  @param[in] unsigned char, unsigned int
  @param[Out] Returns 1 when interrupt is generated.
*/
void TIMER_put_delay(unsigned short timer, unsigned long clocks)
{
    volatile unsigned int *TimersRawIntStatusReg = (volatile unsigned int *)(TIMER_INTR_STATUS_ADDRESS); 	// Global Raw Intr Status Reg

    TIMER(timer).control = 0x0;			// Disable timer.
    TIMER(timer).load_count = clocks;	// Load timer with no of clocks.
    TIMER(timer).control = 0x7;			// Enable timer with intr masked (0111)

    __asm__ __volatile__ ("fence");

    //while(TIMER(timer_no).IntrStatus != 1); 	// Wait for interrupt status bit to set.
    //wEOI = TIMER(timer_no).EOI;			// Reads the EOI register to clear the intr.

    if(timer == 0){
        while((*TimersRawIntStatusReg & 1) != 1)	// Wait till TIMER0 interrupts.
        TIMER(timer).control = 0;			// Disable timer.
	}
    if(timer == 1){
        while((*TimersRawIntStatusReg & 2) != 2)	// Wait till TIMER1 interrupts.
        TIMER(timer).control = 0;			// Disable timer.
	}
    if(timer == 2){
        while((*TimersRawIntStatusReg & 4) != 4)	// Wait till TIMER2 interrupts.
            // Investigate: Should it AND and compare with 4 or 3 ?
        TIMER(timer).control = 0;			// Disable timer.
	}
}


/** @fn void timer_run_in_intr_mode(unsigned char timer_no, unsigned int no_of_clocks)
  @brief  Enable timer interrupt in unmasked & user defined mode..
  @details The selected timer's interrupt is enabled. 
  The LoadCount will  be reloaded with no_of_clocks once the timer is expired and if timer is not disabled.

  @param[in] unsigned char timer_no: Selected timer, unsigned int no_of_clocks : value to be loaded to LoadCount register
  @param[Out] No output parameter.
*/
void TIMER_run_in_intr_mode(unsigned short timer, unsigned long clocks) {

    TIMER(timer).control = 0x0;			// Disable timer.
    TIMER(timer).load_count = clocks;	// Load timer with no of clocks.
    TIMER(timer).control = 0x3;			// Enable timer with intr unmasked. (0011)

	__asm__ __volatile__ ("fence");
}


/** @fn void timer_disable(unsigned char timer_no)
  @brief  Disable timer  
  @details The selected timer and its interrupt is disabled.

  @param[in] unsigned char timer_no: Selected timer.
  @param[Out] No output parameter.
*/
void timer_disable(unsigned char timer_no) {

    TIMER(timer_no).control = 0x0;			// Disable timer.
	__asm__ __volatile__ ("fence");	
}


/** @fn void timer_enable(unsigned char timer_no)
  @brief  Enable timer with interrupt 
  @details The selected timer and its interrupt is enabled and unmasked.

  @param[in] unsigned char timer_no: Selected timer.
  @param[Out] No output parameter.
*/
void timer_enable(unsigned char timer_no) {

    TIMER(timer_no).control = 0x03;			// Enable timer with intr unmasked.
	__asm__ __volatile__ ("fence");	
}

/** @fn void timer_register_isr(unsigned char timer_no, void (*timer_isr)())
  @brief  Enable timer interrupt in interrupt controller module.
  @details Invoke the selected timer ISR.

  @param[in] unsigned char timer_no: Selected timer, function pointer to ISR.
  @param[Out] No output parameter.
*/
void timer_register_isr(unsigned char timer_no, void (*timer_isr)()){ ///*timer_isr is function pointer to user defined intr handler

    unsigned char irq_no;
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



