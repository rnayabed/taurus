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
#define TIMER_acknowledge_interrupt(n) TIMER(n).EOI;

void TIMER_setup_delay(unsigned char timer, unsigned long clocks);

#define TIMER_setup_delay_millis(timer, millis, handler) \
    TIMER_setup_delay(timer, (SYSTEM_FREQUENCY * seconds) / 1000)

#define TIMER_setup_delay_seconds(timer, seconds) \
    TIMER_setup_delay(timer, SYSTEM_FREQUENCY * seconds)

void TIMER_setup_interrupt(unsigned char timer, unsigned long clocks, void (*handler)());

#define TIMER_setup_interrupt_millis(timer, millis, handler) \
    TIMER_setup_interrupt(timer, (SYSTEM_FREQUENCY * seconds) / 1000, handler)

#define TIMER_setup_interrupt_seconds(timer, seconds, handler) \
    TIMER_setup_interrupt(timer, SYSTEM_FREQUENCY * seconds, handler)

void TIMER_register_isr(unsigned char timer, void (*timer_isr)());
void TIMER_enable(unsigned char timer);
void TIMER_disable(unsigned char timer);

#endif /* TIMER_H */
