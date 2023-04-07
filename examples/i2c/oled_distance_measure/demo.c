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
#include "gpio.h"
#include "uart.h"



long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void main() {
	char distance_str[100]={0,};
	clock_t total_time=0;
	UL distance=0;


	printf("\n\r *****************************************************************************");
	printf("\n\r INFO: Connect SSD1306 OLED module to I2C0 ");
	printf("\n\r INFO: Connect ultrasound sensor [HC-SR04] to GPIO 0 ");	
	printf("\n\r INFO: Please try to put an object infront of ultrasound sensor");	
	printf("\n\r INFO: The distance will displayed in OLED Display");	
	printf("\n\r *****************************************************************************\n\r\n\r");

	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

	fill_logo();
	ssd1306_display();
	ssd1306_clearDisplay();
	udelay(500000);
	while(1)
	{

		GPIO_write_pin(0,LOW);
			GPIO_write_pin(0,HIGH);				
			udelay(10);
			GPIO_write_pin(0,LOW);	
			total_time = pulse_duration(1,HIGH);				
			distance = microsecondsToCentimeters(total_time);
			sprintf(distance_str, "Distance :%4d cm",distance);		
		ssd1306_clearDisplay();
		/*char* text = "VEGA series of microprocessors are developed as part of the Microprocessor Development Programme initiated and funded by MeitY";*/
		ssd1306_drawString(distance_str);
		ssd1306_display();
		udelay(5000);		
		
	}
}
