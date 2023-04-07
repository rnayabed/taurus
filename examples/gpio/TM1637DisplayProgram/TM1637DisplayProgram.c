/***************************************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  TM1637 7-Segment Display Demo

  Copyright (C) 2020  CDAC(T). All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/
/**
 @file TM1637DisplayProgram.c
 @brief Demo program for 7-segment display
*/

#include "stdlib.h"
#include "tm1637.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include <string.h>

#define GPIO0 0
#define GPIO1 1
/** 
 * @brief Demo program for 7-segment display
 * @details 
 * @param[in] No input parameter
 * @param[Out] No output parameter 
*/
void main()
{
	printf("+-----[ TM1637 7-Segment Display Demo ]-----+\n");
	printf("|                                           |\n");
	printf("|             DIO ---> GPIO0                |\n");
	printf("|             CLK ---> GPIO1                |\n");
	printf("|                                           |\n");
	printf("+-------------------------------------------+\n\n");
	tm1637 myDisplay;
	myDisplay.DioPin = GPIO0;
	myDisplay.clkPin = GPIO1;
	myDisplay.brightness = 0x0f;

	char data[] = {0x06,0x5B,0x4F,0x66}; 
	printf("Expect : 1234\n");
	setSegments(data, myDisplay); // Expect 1234
	udelay(500000);
	setDots(data);
	printf("Expect : 12:34\n");
	setSegments(data, myDisplay); // Expect 12:34
	udelay(500000);
	clearTM1637(myDisplay);
	printf("Expect : String Animation\n");
	char sampleString[] = "HELLO ON OFF TRUE FALSE ERROR 1234567890";
	while (1)
	{
		displayString(myDisplay,sampleString,strlen(sampleString));	
	}

	while (1);
}
