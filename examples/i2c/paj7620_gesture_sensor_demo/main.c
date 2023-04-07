/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 30-Aug-2021
 Filename		: main.c
 Purpose		: PAJ7620 gesture sensor demo
 Description	: Sample PAJ7620 with I2C interface
 Author(s)	   	: Thomas Sebastian
 Email			: vega@cdac.in
 
 See LICENSE for license details.
 ***************************************************/

/**
 @file main.c
 @brief contains routines for I2C PAJ7620 interface
 @detail Includes software functions declarations to initialize,
 configure, write and read  PAJ7620 over I2C interface
 */

#include "i2c.h"
#include "stdlib.h"
#include "i2c_api.h"
#include "paj7620.h"
#include "gpio.h"

#define TRUE 1
#define FALSE 0

#define GES_REACTION_TIME 50000 // You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME 80000	// When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s).
#define GES_QUIT_TIME 100000

void main()
{
	unsigned char error = 0;

	printf("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

	error = paj7620Init(); // initialize Paj7620 registers

	if (error)
	{
		printf("INIT ERROR,CODE:%x", error);
	}
	else
	{
		printf("\nINIT OK");
	}
	printf("\nPlease input your gestures:\n");

	while (1)
	{
		unsigned char data = 0, data1 = 0, error;

		error = paj7620ReadReg(0x43, 1, &data); // Read Bank_0_Reg_0x43/0x44 for gesture result.

		if (!error)
		{
			switch (data)
			{ // When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
			case GES_RIGHT_FLAG:
				usleep(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG)
				{
					printf("\nForward");
					usleep(GES_QUIT_TIME);
				}
				else if (data == GES_BACKWARD_FLAG)
				{
					printf("\nBackward");
					usleep(GES_QUIT_TIME);
				}
				else
				{
					printf("\nRight");
				}
				break;
			case GES_LEFT_FLAG:
				usleep(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG)
				{
					printf("\nForward");
					usleep(GES_QUIT_TIME);
				}
				else if (data == GES_BACKWARD_FLAG)
				{
					printf("\nBackward");
					usleep(GES_QUIT_TIME);
				}
				else
				{
					printf("\nLeft");
				}
				break;
			case GES_UP_FLAG:
				usleep(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG)
				{
					printf("\nForward");
					usleep(GES_QUIT_TIME);
				}
				else if (data == GES_BACKWARD_FLAG)
				{
					printf("\nBackward");
					usleep(GES_QUIT_TIME);
				}
				else
				{
					printf("\nUp");
				}
				break;
			case GES_DOWN_FLAG:
				usleep(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG)
				{
					printf("\nForward");
					usleep(GES_QUIT_TIME);
				}
				else if (data == GES_BACKWARD_FLAG)
				{
					printf("\nBackward");
					usleep(GES_QUIT_TIME);
				}
				else
				{
					printf("\nDown");
				}
				break;
			case GES_FORWARD_FLAG:
				printf("\nForward");
				usleep(GES_QUIT_TIME);
				break;
			case GES_BACKWARD_FLAG:
				printf("\nBackward");
				usleep(GES_QUIT_TIME);
				break;
			case GES_CLOCKWISE_FLAG:
				printf("\nClockwise");
				break;
			case GES_COUNT_CLOCKWISE_FLAG:
				printf("\nanti-clockwise");
				break;
			default:
				paj7620ReadReg(0x44, 1, &data1);
				if (data1 == GES_WAVE_FLAG)
				{
					printf("\nwave");
				}
				break;
			}
		}
		usleep(10000);
	}
}
