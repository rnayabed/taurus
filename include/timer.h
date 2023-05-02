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
    volatile unsigned int load_count;
    volatile unsigned int current_value;
    volatile unsigned int control;
    volatile unsigned int EOI;
    volatile unsigned int interrupt_status;
} TIMER_Type;


#define TIMER(n) (*((volatile TIMER_Type *)(TIMER_BASE_ADDRESS + (n * sizeof(TIMER_Type)))))

void TIMER_put_delay(unsigned short timer, unsigned long clocks);
void TIMER_run_in_intr_mode(unsigned short timer, unsigned long clocks);


void TIMER_unmask_intr(unsigned char timer_no);
void TIMER_load(unsigned char timer_no,unsigned int count);
void TIMER0_intr_handler(void);
void TIMER1_intr_handler(void);
void TIMER2_intr_handler(void);
void TIMER_register_isr(unsigned char timer_no, void (*timer_isr)());
void TIMER_disable(unsigned char timer_no);
void TIMER_enable(unsigned char timer_no);



#endif /* TIMER_H_ */
