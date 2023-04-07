/***************************************************


 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Jul-2020
 Filename		: demo.c
 Purpose		: I2C Display demo main
 Description		: I2C Display demo main
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/

/******
Demo for ssd1306 i2c driver for  VEGA Processor
******/

#include "ssd1306.h"
#include "stdlib.h"

void main() {
		printf("\n\r *****************************************************************************");
		printf("\n\r INFO: Connect SSD1306 OLED module to I2C0 ");			
		printf("\n\r *****************************************************************************");
		

		ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

		fill_logo();
		ssd1306_display();
		ssd1306_clearDisplay();
		udelay(500000);
	while(1)
	{
		ssd1306_clearDisplay();
		char* text = "VEGA series of microprocessors are developed as part of the Microprocessor Development Programme initiated and funded by MeitY";
		ssd1306_drawString(text);
		ssd1306_display();
		udelay(500000);

		fill_logo();
		ssd1306_display();
		ssd1306_clearDisplay();
		udelay(500000);
		
		ssd1306_startscrollleft(00,0xFF);
		udelay(500000);
		ssd1306_stopscroll();

		ssd1306_clearDisplay();
		ssd1306_fillRect(10,10, 50, 20, WHITE);
		ssd1306_fillRect(80, 10, 130, 50, WHITE);
		ssd1306_display();
		udelay(500000);
		
	}
}
