/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 28-april-2021
 Filename		: led_dot_matrix_scroll_demo.c
 Purpose		: spi led_dot_matrix_scroll_demo
 Description	: Sample dot matrix with spi interface
 Author(s)		: Anoop Varghese, C-DAC
 Email			: vanoop@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @brief max7219 demo.
 @details interface max7219 with led dot matrix with spi .
 @param[in] No input parameter. 
 @param[Out] No output parameter. 
*/

#include "stdlib.h"
#include "spi.h"
#include "max7219.h"

void main()
{
	max7219Begin(0);
	max7219ClearScreen();

	max7219_Brightness(0x0a); // min val 0x01 ,, max val 0xff

	printf("+---------[ MAX7219 Demo ]--------+\n");
	printf("|                                 |\n");
	printf("|  MAX7219 DOT MATRIX ----> SPI0  |\n");
	printf("|                                 |\n");
	printf("+---------------------------------+\n\n");

	printf("INFO : printing  VEGA MICROPROCESSORS  \n");

	while (1)
	{

		max7219ScrollText("VEGA MICROPROCESSORS ", 12000); // (text for scrolling,8000,text len)
	}
}
