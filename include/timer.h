/** @file timer.h
 *  @brief Header file with functions to use hardware timers.
 *
 *  @author Debayan Sutradhar
 *
 *  SPDX-License-Identifier: MIT
 */



#ifndef TIMER_H
#define TIMER_H

#include "config.h"

#define 	TIMER_0		0
#define 	TIMER_1		1
#define 	TIMER_2		2

typedef struct
{
    unsigned int load_count;
    unsigned int current_value;
    unsigned int control;
    unsigned int EOI;
    unsigned int interrupt_status;
} TimerReg_type;


#define TIMER(n) (*((volatile TimerReg_type *)(TIMER_BASE_ADDRESS + (n * sizeof(TimerReg_type)))))

void TIMER_put_delay(unsigned short timer, unsigned long clocks);
void TIMER_run_in_intr_mode(unsigned short timer, unsigned long clocks);


void TIMER_unmask_intr(unsigned char timer_no);
void timer_load(unsigned char timer_no,unsigned int count);
void timer0_intr_handler(void); 	
void timer1_intr_handler(void);
void timer2_intr_handler(void);
void timer_register_isr(unsigned char timer_no, void (*timer_isr)());
void timer_disable(unsigned char timer_no);
void timer_enable(unsigned char timer_no);



#endif /* TIMER_H_ */
