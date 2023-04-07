/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Nov-2019
 Filename		: debug_uart.h
 Purpose		: Common interface for uart
 Description		: UART tx/rx functions
 Author(s)		: Premjith A V, Sreenadh S
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/
/**
 @file debug_uart.h
 @brief header file for debug_uart 
 @detail 
 */

#ifndef DEBUG_UART_H_
#define DEBUG_UART_H_

#include "config.h"
#include "stdlib.h"
/**
 *  Definition section
***************************************************/

typedef struct uart_reg
{
	UI   UART_DR; 	/*0x00*/
	UI   UART_IE; 	/*0x04*/
	UI   UART_IIR; 	/*0x08*/
	UI   UART_LCR; 	/*0x0c*/
	UI   Dummy10; 	/*0x10*/
	UI   UART_LSR; 	/*0x14*/
}UART_REG;

/**
 *   Function declaration section
***************************************************/

void init_uart(void);
void tx_uart(UC tx_char);
UC rx_uart(void);
UL get_long_int(UC noofBytes);
UC get_hex();
UL get_decimal(UC noOfDigits);

#define uart_regs (*((volatile UART_REG *)(UART_BASE)))

#endif /* DEBUG_UART_H_ */
