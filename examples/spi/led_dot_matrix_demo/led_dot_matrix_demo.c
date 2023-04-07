/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 28-april-2021
 Filename		: led_dot_matrix_demo.c
 Purpose		: spi led_dot_matrix_demo
 Description	: Sample dot matrix with spi interface
 Author(s)		: Thomas Sebastian, C-DAC
 Email			: vega@cdac.in

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

	unsigned long long IMAGES[] = {
		0x505020a870200000,
		0x505120a870200000,
		0x525020a870200000,
		0x545020a870200000,
		0x585020a870200000,
		0x485420a870200000,
		0x505022a870200000,
		0x505020a970200000,
		0x505020a970200000,
		0x505020a872200000,
		0x505020a870240000,
		0x505020a870200800,
		0x505020a870200010,
		0x505020a870202000,
		0x505020f870000000,
		0x505020f870000000,
		0x5050f87020000000,
		0x50f8702000000000,
		0xf870200000000000};

	const int IMAGES_LEN = sizeof(IMAGES) / 8;

	while (1)
	{
		for (int i = 0; i < IMAGES_LEN; i++)
		{
			max7219PrintScreen(IMAGES + i);
			udelay(30000);
		}
	}
}
