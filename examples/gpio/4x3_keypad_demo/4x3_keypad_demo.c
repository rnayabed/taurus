

/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 16-aprl-2021
 Filename		: 4x3_keypad_demo.c
 Purpose		: interfacing 4x3_keypad with vega brd
 Description	: interfacing 4x3_keypad via GPIO pins to vega board
 Author(s)		: thomas sebastian
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/

#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

#define NO_CHAR '$'
#define KEYPAD_START 3

/**
 @file 4x3_keypad_demo.c
 @brief Contains routines for read data from keypad using 1-wire protocol
 */

/** 
 * @brief serial print  based on the switch status.
 * @details In this pgm  keypad is interfaced with vega board                                      .
 * @warning connect pulldown resistor(1k) with gpio pin 7,8,9 with gnd.
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/

char keypad();    		// keypad function

void main()
{
	UC uart_error;
	UC repeat, rx = 0;
	char key;
	
	while (1)
	{
		
		key = keypad();
		if (key != NO_CHAR)
			printf(" Key : %c\n", key); 		

		udelay(12000);
	}
}
char keypad()
{
	/*
	@  current code req pulldwn res (1k) conctd to the column pins to be ground, and connect  it with gpio pins 7,8,9 respc..
	@if u need to avoid the pulldwn res , change the values --> init_val=0,req_val=1 <--  to --> init_val=1,req_val=0 <--  respec....
	@ if u use --> init_val=1,req_val=0 <-- format then connect the column pins directly to gpio pins 7,8 & 9 .
	*/

	UI init_val=0,req_val=1; 	
	

	GPIO_write_pin(3, init_val);
	GPIO_write_pin(4, init_val);
	GPIO_write_pin(5, init_val);
	GPIO_write_pin(6, init_val);

	for (char i = 0; i < 4; i++)
	{
		GPIO_write_pin(7, init_val);
		GPIO_write_pin(8, init_val);
		GPIO_write_pin(9, init_val);
		
		//printf("Data[%d] : %d %d %d \n",i,GPIO_read_pin(7),GPIO_read_pin(8),GPIO_read_pin(9));

		GPIO_write_pin(KEYPAD_START + i, req_val);

		//printf("Data[%d] : %d %d %d \n\n\n",i,GPIO_read_pin(7),GPIO_read_pin(8),GPIO_read_pin(9));

		if (i < 3)
		{
			if (GPIO_read_pin(7) == req_val)
				return ('1' + (i * 3));
			if (GPIO_read_pin(8) == req_val)
				return ('1' + (i * 3) + 1);
			if (GPIO_read_pin(9) == req_val)
				return ('1' + (i * 3) + 2);
		}
		else
		{
			if (GPIO_read_pin(7) == req_val)
				return ('*');
			if (GPIO_read_pin(8) == req_val)
				return ('0');
			if (GPIO_read_pin(9) == req_val)
				return ('#');
		}

		GPIO_write_pin(i, init_val);
	}
	usleep(80000);

	return '$';
}
