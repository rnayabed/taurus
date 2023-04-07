
#ifndef __UART_H
#define __UART_H
/**
 @file uart.h
 @brief header file for UART driver
 @detail 
 */
/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Nov-2019
 Filename		: uart.h
 Purpose		: UART header file
 Description		: uart functions
 Author(s)		: Karthika P
 Email			: karthikap@cdac.in

 See LICENSE for license details.
 ***************************************************/

/*  Include section
*
*
***************************************************/

//#include "registers.h"


/*  Defines section
*
*
***************************************************/

typedef unsigned char UC; //1 Byte
typedef unsigned short US; //2 Bytes
typedef unsigned int UI; //4 Bytes
typedef unsigned long UL; //8 Bytes


typedef struct
{
	UI UART_DR;
	UI UART_IE;
	UI UART_IIR_FCR;
	UI UART_LCR;
	UI Dummy;
	UI UART_LSR;
}UART_REG_TYPE;


#define UART_0		0
#define UART_1		1
#define UART_2		2

#define UART_NO_ERROR 0
#define UART_PARITY_ERROR -1
#define UART_OVERRUN_ERROR -2
#define UART_FRAMING_ERROR -3


#define TX_ENABLE	1
#define TX_DISABLE	0
#define RX_ENABLE	1
#define RX_DISABLE	0

//Register address mapping

#define UartReg(i) (*((volatile UART_REG_TYPE *)(UART_BASE+ (0x100 * i))))
/*  Function declaration section
*
***************************************************/

void uart_init(UC uart_number);
void uart_configure(UC uart_number, UL Baud_rate, UL frame_value, UL Uart_clock);
void uart_putchar(UC uart_number, UC bTxCharacter, char *error);
UC uart_getchar(UC uart_number, char *error);
void uart_intr_enable(UC uart_number, UC tx_intr, UC rx_intr);
void uart_set_baud_rate(UC uart_number,UL Baud_rate, UL Uart_clock);
//int uart_intr_handler(UC uart_number);
void uart_intr_enable(UC uart_number, UC tx_intr, UC rx_intr);
#endif /*__UART_H*/


