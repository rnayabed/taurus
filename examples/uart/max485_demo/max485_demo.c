/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 23-july-2021
 Filename		: max485_demo.c
 Purpose		: interface max485
 Description		: transmission & reception with uart interface with max485
 Author(s)		: Thomas sebastian
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file max485_demo.c
 @brief Contains routines for UART interface Includes software functions to initialize, configure, transmit and receive over UART
 */

/*  Include section
 ***************************************************/

#include "uart.h"
#include "stdlib.h"
#include "config.h"
#include "gpio.h"

#define LOW 0
#define HIGH 1

/**
 @fn main
 @brief transmit and reception through uart
 @details 1 string is transmitted and received through uart
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
		/*if (*str == '\n')
		uart_putchar(UART_1, '\r', &error);*/
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
		//printf("%x",data);
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
	char str[100] = {
		0,
	};

	uart_set_baud_rate(UART_1, 9600, SYSTEM_FREQUENCY); // set baud rate

	printf("RS485 DEMO \n\r");
	printf("Connect RE and DE to GPIO 0 pin \n\r");

	GPIO_write_pin(0, HIGH); // for initiation

	while (1)
	{
		GPIO_write_pin(0, LOW); // RE & DE is set LOW for Receiving data
		receive_string(str);	// Receive data
		printf("%s\n", str);

		GPIO_write_pin(0, HIGH); // RE & DE is set HIGH for Transmitting data
		send_string("DEF");		 //Transmit data
		

		udelay(1000);
	}
}
