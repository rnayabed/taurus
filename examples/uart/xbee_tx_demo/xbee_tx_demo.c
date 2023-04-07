/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 15-sep-2021
 Filename		: xbee_tx_demo.c
 Purpose		: UART interface with xbee
 Description		: transmission & reception with uart interface
 Author(s)		: Thomas Sebastian
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file xbee_tx_demo.c
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
 @details characters are transmitted and received through uart
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
int send_string(char *str) // function for send string
{
	char error;
	while (*str != '\0')
	{
		uart_putchar(UART_1, *str, &error); // send single character
		str++;
	}
	return 0;
}

void receive_string(char *str) // function for receive string
{
	char error;
	char data;
	while (1)
	{
		data = uart_getchar(UART_1, &error); // receive single character
		printf("%c", data); // print single character
		*str = data;
		str++;
		if (data == '\0')
		{
			*str = '\r';
			break;
		}
	}
}

void main()
{
	char error;
	UC data;
	char str[100] = {
		0,
	};

	uart_set_baud_rate(UART_1, 9600, SYSTEM_FREQUENCY); 

	printf("testing uart \n\r");

	while (1)
	{
/*		data = uart_getchar(UART_1, &error); // receive single character
		printf("%x",data);
*/

/*	uart_putchar(UART_1, 'v', &error); // send single character
	uart_putchar(UART_1, 'e', &error); // send single character
	uart_putchar(UART_1, 'g', &error); // send single character
	uart_putchar(UART_1, 'a', &error); // send single character
	uart_putchar(UART_1, '\n', &error); // send single character
*/

		send_string("vega processors\n"); //Transmit data

		udelay(100000);
	}
}
