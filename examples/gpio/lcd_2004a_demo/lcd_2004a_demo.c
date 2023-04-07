

/***************************************************
* Module name: main.c
*
* Copyright 2020 Company CDAC(T)
* All Rights Reserved.
*
*  The information contained herein is confidential
* property of Company. The user, copying, transfer or
* disclosure of such information is prohibited except
* by express written agreement with Company.
*
*
* Module Description:
* LCD Display, hello world program
*
***************************************************/

/*  Include section
*
*
***************************************************/

#include "lcd.h"

/**
 @file lcd_2004a_demo.c
 @brief Main program to display text in 20*4 LCD display
 */

/** 
 * @brief To display text string in LCD display
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main ()
{	
	printf("+--------[ LCD (2004A) display Demo ]-------+\n");
	printf("|                                           |\n");
	printf("|             RS--->GPIO_3	            |\n");	
	printf("|             RW--->GPIO_4	            |\n");
	printf("|         ENABLE--->GPIO_5                  |\n");
	printf("|            DB4--->GPIO_6	            |\n");
	printf("|            DB5--->GPIO_7	            |\n");
	printf("|            DB6--->GPIO_8	            |\n");
	printf("|            DB7--->GPIO_9	            |\n");
	printf("|                                           |\n");
	printf("+-------------------------------------------+\n\n");

	lcdInit();
	setCursor(0,0);
	lcdPrint("Hello World");
	setCursor(2,1);
	lcdPrint("Hello World");	
	setCursor(4,2);
	lcdPrint("Hello World");	
	setCursor(8,3);
	lcdPrint("Hello World");		
}




