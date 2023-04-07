/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 30-Apr-2021
 See LICENSE for license details.
 ***************************************************/
/**
 @file software_serial.c
 @brief Contains routines for Software based uart
 */
 
/*  Include section
 ***************************************************/

#include "uart.h"
#include "stdlib.h"
#include "gpio.h"
#include "config.h"
#include "soft_uart.h"

#define DELAY 1000000
/**
 @fn main
 @brief transmit and reception through uart
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
void main() {

	char str[100] = { 0, };//receive buffer
	
	printf("\n\r *****************************************************************************");	
	printf("\n\r SoftUART 9600 BAUD : GPIO_0 - RX, GPIO_1 -TX");	
	printf("\n\r *****************************************************************************\n\r");	
	
	soft_uart1(GPIO_1, GPIO_0); //GPIO_1 as TX, GPIO_0 as RX

	soft_uart_send("VEGA Processor\r\nCDAC\r\n");
	
	soft_uart_receive(str);
	printf("STR: %s\n", str);

	while(1);

}

