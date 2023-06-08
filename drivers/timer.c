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


/** @fn unsigned char TIMER_setup_delay(unsigned char timer, unsigned long clocks)
  @brief load, enable and wait for interrupt (Polling mode).
  @details The selected timer is loaded with the no of clocks and it is enabled with intr masked.
  The timer module waits untill it interrupts.

  @param[in] unsigned short timer: Selected timer, unsigned long clocks: Number of clock ticks to wait
  @param[Out] No output parameter.
*/
void TIMER_setup_delay(unsigned char timer, unsigned long clocks)
{
    TIMER(timer).control = 0x0;             // Disable timer.
    TIMER(timer).load_count = clocks / 2;   // Load timer with no of clocks.
    TIMER(timer).control = 0x7;             // Enable timer with intr masked (0111)

    __asm__ __volatile__ ("fence");

    while(!TIMER_INTR_FLAG(timer));         // Wait till timer interrupts.
    TIMER(timer).control = 0x0;             // Disable timer.

    __asm__ __volatile__ ("fence");
}


/** @fn void TIMER_setup_interrupt(unsigned char timer, unsigned long clocks, void (*handler)())
  @brief  Enable timer interrupt in unmasked & user defined mode..
  @details The selected timer's interrupt is enabled. 
  The LoadCount will  be reloaded with no_of_clocks once the timer is expired and if timer is not disabled.

  @param[in] unsigned char timer: Selected timer, unsigned int no_of_clocks : value to be loaded to LoadCount register
  @param[Out] No output parameter.
*/
void TIMER_setup_interrupt(unsigned char timer, unsigned long clocks,
                                  void (*handler)(void))
{
    unsigned char irq_no = 7;
#if __riscv_xlen == 64
    if(timer == 0)
        irq_no = 10;
    else if(timer == 1)
        irq_no = 11;
    else if(timer == 2)
        irq_no = 12;

#else
    if(timer == 0)
        irq_no = 7;
    else if(timer == 1)
        irq_no = 8;
    else if(timer == 2)
        irq_no = 9;
#endif
    interrupt_enable(irq_no);
    irq_register_handler(irq_no, handler);

    __asm__ __volatile__ ("fence");

    TIMER(timer).control = 0x0;			// Disable timer.
    TIMER(timer).load_count = clocks;	// Load timer with no of clocks.
    TIMER(timer).control = 0x3;			// Enable timer with intr unmasked. (0011)

	__asm__ __volatile__ ("fence");
}


/** @fn void TIMER_enable(unsigned char timer)
  @brief  Enable timer with interrupt
  @details The selected timer and its interrupt is enabled and unmasked.

  @param[in] unsigned char timer_no: Selected timer.
  @param[Out] No output parameter.
*/
void TIMER_enable(unsigned char timer) {

    TIMER(timer).control = 0x3;			// Enable timer with intr unmasked.
    __asm__ __volatile__ ("fence");
}


/** @fn void TIMER_disable(unsigned char timer)
  @brief  Disable timer  
  @details The selected timer and its interrupt is disabled.

  @param[in] unsigned char timer_no: Selected timer.
  @param[Out] No output parameter.
*/
void TIMER_disable(unsigned char timer) {

    TIMER(timer).control = 0x0;			// Disable timer.
	__asm__ __volatile__ ("fence");	
}

