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
#include "i2c.h"
#include <string.h>
#include "ds3231_rtc.h"


/***************************************************


 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Nov-2019
 Filename		: i2c_RTC.c
 Purpose		: I2C RTC interface
 Description		: Sample RTC with I2C interface
 Author(s)		: Karthika P
 Email			: karthikap@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file i2c_EEPROM.c
 @brief contains routines for I2C EEPROM interface
 @detail Includes software functions declarations to initialize,
 configure, write and read EEPROM over I2C interface
 */

#include "i2c.h"
#include "gpio.h"
#include "stdlib.h"
#include "ds3231_rtc.h"

void getPIRinfo (char * pir_str)
{

	US pin_data0 = 0;
	UC repeat,rx = 0;
	
	
		repeat = 0;


		

		while(!repeat)
		{
			//rx=0;
			pin_data0 = GPIO_read_pin(0); //SW0 LSB
			
			if((pin_data0 == 1) )
			{
				if (rx==0)		{
				printf("\rO");
				ssd1306_clearDisplay();		
				//ssd1306_drawString(pir_str);
				ssd1306_fillRect(10, 10, 100, 100, WHITE);
				ssd1306_display();
				rx=1;
			     }
			}

			else
			{	

			if(rx==1){
				printf("\rX");
				rx=0;
				ssd1306_clearDisplay();		
				//ssd1306_drawString(pir_str);
				ssd1306_fillRect(10, 10, 10, 10, WHITE);
				ssd1306_display();
			       }
			
			
			}
			//udelay(1000);
		}
	



}
void main() {
char pir_str[100]={0,};
clock_t total_time=0;
	UL distance=0;
		printf("\n\r ***************************************************************************");
		printf("\n\r INFO: Connect SSD1306 OLED module to I2C0 ");
		printf("\n\r INFO: Connect PIR to GPIO 0 ");
		printf("\n\r INFO: Please try to move an object infront of PIR");
		printf("\n\r ***************************************************************************\n\r\n\r");

		ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

		fill_logo();
		ssd1306_display();
		ssd1306_clearDisplay();
		udelay(500000);
	while(1)
	{

		getPIRinfo(pir_str);	
		
		udelay(1000);		
		
	}
}
