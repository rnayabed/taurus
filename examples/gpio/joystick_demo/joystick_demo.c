/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 13-aprl-2021
 Filename		: joystic_demo.c
 Purpose		: demo pgm of 5d rocker joystick
 Description		: compatable with 5D rocker joystick
 Author(s)		: thomas sebastian
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/

#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

/**
 @file joystic_demo.c
 @brief Contains routines for read signal using 1-wire protocol
 */

/** 
 * @brief Lit leds based on the switch status.
 * @details In this pgm --> up,dwn,lft,rht,set&rst <-- are considered for turning LEDs--> LD0,LD1,LD5,LD6 & LD7 <-- ON & OFF.
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main()
{
	UC uart_error;
	US pin_val = 0;
	US pin_up = 1, pin_dwn = 1, pin_lft = 1, pin_rht = 1, pin_mid = 1, pin_rst = 1, pin_set = 1;
	UC repeat, rx = 0;

	while (1)
	{
		repeat = 0;

		printf("\n\r   *************");
		printf("\n\r   GPIO TEST PGM");
		printf("\n\r   *************");
		printf("\n\r INFO: Program will reset on the switch combination 1 1 1");
		printf("\n\r Press any key to Start");
		printf("\n\r ***************************************************************************\n\n");
		rx = uart_getchar(0, &uart_error);

		while (!repeat)
		{

			pin_up = GPIO_read_pin(3);
			pin_dwn = GPIO_read_pin(4);
			pin_lft = GPIO_read_pin(5);
			pin_rht = GPIO_read_pin(6);
			pin_mid = GPIO_read_pin(7);
			pin_set = GPIO_read_pin(8);
			pin_rst = GPIO_read_pin(9);

			if (pin_set == 0)
			{

				printf("\r SET BUTTON is pressed !!");

				GPIO_write_pin(19, OFF_LED);
				GPIO_write_pin(20, OFF_LED);
				GPIO_write_pin(21, OFF_LED);

				GPIO_write_pin(22, OFF_LED);
				GPIO_write_pin(23, OFF_LED);
				GPIO_write_pin(24, OFF_LED);

				GPIO_write_pin(16, OFF_LED);
				GPIO_write_pin(17, OFF_LED);
				GPIO_write_pin(18, OFF_LED);
			}
			if (pin_up == 0)
			{
				printf("\r UP BUTTON is pressed !!");

				GPIO_write_pin(19, OFF_LED);
				GPIO_write_pin(20, OFF_LED);
				GPIO_write_pin(21, OFF_LED);

				GPIO_write_pin(22, ON_LED);
				GPIO_write_pin(23, OFF_LED);
				GPIO_write_pin(24, OFF_LED);

				GPIO_write_pin(16, OFF_LED);
				GPIO_write_pin(17, OFF_LED);
				GPIO_write_pin(18, OFF_LED);
			}
			if (pin_dwn == 0)
			{
				printf("\r DOWN BUTTON is pressed !!");

				GPIO_write_pin(19, OFF_LED);
				GPIO_write_pin(20, OFF_LED);
				GPIO_write_pin(21, OFF_LED);

				GPIO_write_pin(22, OFF_LED);
				GPIO_write_pin(23, OFF_LED);
				GPIO_write_pin(24, ON_LED);

				GPIO_write_pin(16, OFF_LED);
				GPIO_write_pin(17, OFF_LED);
				GPIO_write_pin(18, OFF_LED);
			}

			if (pin_lft == 0)
			{
				printf("\r LEFT BUTTON is pressed !!");

				GPIO_write_pin(19, ON_LED);
				GPIO_write_pin(20, OFF_LED);
				GPIO_write_pin(21, OFF_LED);

				GPIO_write_pin(22, OFF_LED);
				GPIO_write_pin(23, OFF_LED);
				GPIO_write_pin(24, OFF_LED);

				GPIO_write_pin(16, OFF_LED);
				GPIO_write_pin(17, OFF_LED);
				GPIO_write_pin(18, OFF_LED);
			}
			if (pin_rht == 0)
			{
				printf("\r RIGHT BUTTON is pressed !!");

				GPIO_write_pin(19, OFF_LED);
				GPIO_write_pin(20, OFF_LED);
				GPIO_write_pin(21, ON_LED);

				GPIO_write_pin(22, OFF_LED);
				GPIO_write_pin(23, OFF_LED);
				GPIO_write_pin(24, OFF_LED);

				GPIO_write_pin(16, OFF_LED);
				GPIO_write_pin(17, OFF_LED);
				GPIO_write_pin(18, OFF_LED);
			}

			if (pin_mid == 0)
			{
				printf("\r MIDDLE BUTTON is pressed !!");

				GPIO_write_pin(19, OFF_LED);
				GPIO_write_pin(20, ON_LED);
				GPIO_write_pin(21, OFF_LED);

				GPIO_write_pin(22, OFF_LED);
				GPIO_write_pin(23, ON_LED);
				GPIO_write_pin(24, OFF_LED);

				GPIO_write_pin(16, OFF_LED);
				GPIO_write_pin(17, OFF_LED);
				GPIO_write_pin(18, OFF_LED);
			}

			if (pin_rst == 0)
			{
				printf("\r RESET BUTTON is pressed !!");

				GPIO_write_pin(19, OFF_LED);
				GPIO_write_pin(20, OFF_LED);
				GPIO_write_pin(21, OFF_LED);

				GPIO_write_pin(22, OFF_LED);
				GPIO_write_pin(23, OFF_LED);
				GPIO_write_pin(24, OFF_LED);

				GPIO_write_pin(16, ON_LED);
				GPIO_write_pin(17, ON_LED);
				GPIO_write_pin(18, ON_LED);

				udelay(15000);

				GPIO_write_pin(19, OFF_LED);
				GPIO_write_pin(20, OFF_LED);
				GPIO_write_pin(21, OFF_LED);

				GPIO_write_pin(22, OFF_LED);
				GPIO_write_pin(23, OFF_LED);
				GPIO_write_pin(24, OFF_LED);

				GPIO_write_pin(16, OFF_LED);
				GPIO_write_pin(17, OFF_LED);
				GPIO_write_pin(18, OFF_LED);
			}
		}
	}
}
