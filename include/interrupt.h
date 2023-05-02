/** @file timer.h
 *  @brief Header file with functions to use hardware interrupts.
 *
 *  @author Debayan Sutradhar
 *
 *  SPDX-License-Identifier: MIT
 */



#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "stdlib.h"
#include "encoding.h"

#if __riscv_xlen == 64

#define UART_0_IRQ		1
#define UART_1_IRQ		2
#define UART_2_IRQ		3
#define SPI_0_IRQ		6
#define SPI_1_IRQ		7
#define I2C_0_IRQ		8
#define I2C_1_IRQ		9
#define TIMER_0_IRQ		10
#define TIMER_1_IRQ		11
#define TIMER_2_IRQ		12
#define GPIO_0_IRQ		20
#define GPIO_1_IRQ		21
#define GPIO_2_IRQ		22
#define GPIO_3_IRQ		23
#define GPIO_4_IRQ		24
#define GPIO_5_IRQ		25
#define GPIO_6_IRQ		26
#define GPIO_7_IRQ		27
#define GPIO_8_IRQ		28
#define GPIO_9_IRQ		29
#define GPIO_10_IRQ		30
#define GPIO_11_IRQ		31
#define GPIO_12_IRQ		32
#else

#define UART_0_IRQ		0
#define UART_1_IRQ		1
#define UART_2_IRQ		2
#define SPI_0_IRQ		3
#define SPI_1_IRQ		4
#define I2C_0_IRQ		5
#define I2C_1_IRQ		6
#define TIMER_0_IRQ		7
#define TIMER_1_IRQ		8
#define TIMER_2_IRQ		9
#define GPIO_0_IRQ		10
#define GPIO_1_IRQ		11
#define GPIO_2_IRQ		12
#define GPIO_3_IRQ		13
#define GPIO_4_IRQ		14
#define GPIO_5_IRQ		15
#define GPIO_6_IRQ		16
#define GPIO_7_IRQ		17
#define GPIO_8_IRQ		18
#define GPIO_9_IRQ		19
#define GPIO_10_IRQ		20
#define GPIO_11_IRQ		21
#define GPIO_12_IRQ		22


#endif


typedef void (*fp)(void); //Declares a type of a void function that accepts an void

typedef struct
{
#if __riscv_xlen == 64
	UL   RAW_INTR; 		//0x00
	UL   INTR_EN; 		//0x08
	UL   INTR_STATUS; 	//0x10
#else
	UI   RAW_INTR; 		//0x00
    UI   pad1;          //0x04
	UI   INTR_EN; 		//0x08
    UI   pad2;          //0x0c
	UI   INTR_STATUS; 	//0x10
#endif
} INTR_Type;

#define PLIC_INTR_REGS (*((volatile INTR_Type *)PLIC_BASE_ADDRESS))


void enable_irq(void);
void enable_sw_irq(void) ;
void interrupt_enable(UC intr_number);
void irq_register_handler(UC irq_no, void (*irq_handler)());
void sw_irq_register_handler( void (*irq_handler)());
//void interrupt_handler(void);
void interrupt_handler(void);
void sw_interrupt_handler(void);
void generate_sw_irq(void);
void disable_sw_irq(void);
#endif	/* _INTERRUPT_H */	



