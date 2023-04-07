/**
 * \file
 *
 * \brief lcd controller ili9488 example.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage lcd controller ili9488 Example
 *
 * \section Purpose
 *
 * This example demonstrates how to configure lcd controller ili9488
 * to control the LCD on the board.
 *
 * \section Requirements
 *
 * This package can be used with samv71_xplained_ultra evaluation kits.
 *
 * \section Description
 *
 * This example first configure ili9488 for access the LCD controller,
 * then initialize the LCD, finally draw some text, image, basic shapes (line,
 * rectangle, circle) on LCD.
 *
 * \section Usage
 *
 * -# Connect mXT143E Xplained(ILI9488 LCD Controller) to EXT2 on SAME70 Xplained Pro Kit using 20 pin connector.
 * -# Build the program and download it inside the evaluation board.
 * -# Some text, image and basic shapes should be displayed on the LCD.
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "stdlib.h"
#include "ili9488.h"
//#include "stdio_serial.h"
//#include "conf_board.h"
//#include "conf_clock.h"


struct ili9488_opt_t g_ili9488_display_opt;

/** Tick Counter in unit of ms. */
volatile uint32_t g_ul_tick_count;

/** System tick frequency in Hz. */
#define SYS_TICK_FREQ        1000

#define ILI9488_SPIMODE
/**
 * \brief Application entry point for smc_lcd example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	//sysclk_init();
	//board_init();
	
	/* Initialize debug console */
	//configure_console();
	printf("ILI9488 example\n\r");
	
	//configure_systick();

	/* Initialize display parameter */
	g_ili9488_display_opt.ul_width = ILI9488_LCD_WIDTH;
	g_ili9488_display_opt.ul_height = ILI9488_LCD_HEIGHT;
	g_ili9488_display_opt.foreground_color = COLOR_CONVERT(COLOR_WHITE);
	g_ili9488_display_opt.background_color = COLOR_CONVERT(COLOR_WHITE);

	/* Initialize LCD */
	ili9488_init(&g_ili9488_display_opt);
	
	/* Draw text, image and basic shapes on the LCD */
	ili9488_set_foreground_color(COLOR_CONVERT(0x283272));
	ili9488_draw_filled_rectangle(0, 0, ILI9488_LCD_WIDTH,ILI9488_LCD_HEIGHT);
	
	
	ili9488_set_foreground_color(COLOR_CONVERT(0xffffff));
	ili9488_draw_string(85, 450, (uint8_t *)"VEGA Procesor");

	//ili9488_draw_string(10, 50, (uint8_t *)"Indias first Indigenous 64-bit, Multi-core, Out-of-Order, Superscalar RISC-V Processor");

	usleep(1000);
	int x=15,y=10,x1=300,y1=10;
	int xc=10,yc=10,xc1=-10,yc1=-10;
	while(1){
		 
		if(x>300)
			xc=-11;
		else if(x<20)
			xc=9;

		if(y>460)
			yc=-9;
		else if(y<20)
			yc=11;	

		if(x1>300)
			xc1=-9;
		else if(x1<20)
			xc1=11;

		if(y1>460)
			yc1=-11;
		else if(y1<20)
			yc1=9;	


		ili9488_set_foreground_color(COLOR_CONVERT(COLOR_GRAY));
		ili9488_draw_filled_circle(x, y, 10);

		ili9488_set_foreground_color(COLOR_CONVERT(0x283272));
		ili9488_draw_filled_circle(x1, y1, 10);
		x+=xc;
		y+=yc;

		x1+=xc1;
		y1+=yc1;

		ili9488_set_foreground_color(COLOR_CONVERT(0xf5b21b));
		ili9488_draw_filled_circle(x, y, 10);

		//ili9488_set_foreground_color(COLOR_CONVERT(0xf5b21b));
		ili9488_draw_filled_circle(x1, y1, 10);

		usleep(25000);
		
		ili9488_set_foreground_color(COLOR_CONVERT(0xffffff));
		ili9488_draw_string(85, 450, (uint8_t *)"VEGA Procesor");
		
	}
	
	
	//ili9488_set_foreground_color(COLOR_CONVERT(COLOR_VIOLET));
	//ili9488_draw_line(0, 0, ILI9488_LCD_WIDTH, ILI9488_LCD_HEIGHT);
	
	while(1) {

	}
}

