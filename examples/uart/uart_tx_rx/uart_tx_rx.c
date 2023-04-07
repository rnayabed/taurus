/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Nov-2019
 Filename		: uart_tx_rx.c
 Purpose		: UART interface
 Description		: transmission & reception with uart interface
 Author(s)		: Karthika P
 Email			: karthikap@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file uart_tx_rx.c
 @brief Contains routines for UART interface Includes software functions to initialize, configure, transmit and receive over UART
 */

/*  Include section
 ***************************************************/

#include "uart.h"
#include "stdlib.h"
#include "config.h"

/**
 @fn main
 @brief transmit and reception through uart
 @details 1 character is transmitted and received through uart
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
void main()
{
	char error;
	UC data;

	uart_set_baud_rate(UART_0, 115200, SYSTEM_FREQUENCY);
	printf("testing uart \n\r");
	uart_putchar(UART_0, 't', &error);
	if (error)
	{
		switch (error)
		{
		case (char)UART_PARITY_ERROR:
			printf("Parity Error\n\r");
			break;
		case (char)UART_OVERRUN_ERROR:
			printf("Overrun Error\n\r");
			break;
		case (char)UART_FRAMING_ERROR:
			printf("Framing Error\n\r");
			break;
		}
	}
	else
		printf("no error\n\r");
	printf("press any key \n\r");
	data = uart_getchar(UART_0, &error);
	if (error)
	{
		switch (error)
		{
		case (char)UART_PARITY_ERROR:
			printf("Parity Error\n\r");
			break;
		case (char)UART_OVERRUN_ERROR:
			printf("Overrun Error\n\r");
			break;
		case (char)UART_FRAMING_ERROR:
			printf("Framing Error\n\r");
			break;
		}
	}
	else
		printf("no error\n\r");
	printf("data is %d", data);
	printf("\n\r");
	printf("success\n\r");
	while (1)
		;
}
