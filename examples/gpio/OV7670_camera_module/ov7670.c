/***************************************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  OV7670 Camera Module Demo
* Based on                              :  http://embeddedprogrammer.blogspot.com/2012/07/hacking-ov7670-camera-module-sccb-cheat.html

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
 @file ov7670.c
 @brief Main program to capture the image through OV7670 camera module
*/


#include "stdlib.h"
#include "gpio.h"
#include "pwmc.h"

#define SYS_CLK_FREQ SYSTEM_FREQUENCY
#define READ_SLAVE_ADDR 0x43
#define WRITE_SLAVE_ADDR 0x42

/* SCCB gpios*/
#define SIO_C 11
#define SIO_D 12
#define SIO_CLOCK_DELAY 100

/* pixel clock */
#define PCLK (1 << 8)
#define HSYNC (1 << 9)
#define VSYNC (1 << 10)

#define PIXELS_PER_FRAME (174 * 144) /* Default frame size is 174 * 144 pixels and pixels are 2 bytes long YCbCr422. */

#define PIXEL_PINS 0x7FF // GPIO0 - GPIO10 (D0 - D7, PCLK, HSYNC, VSYNC)
/* read pixel data from GPIO 0-10. Could use API functions but to improve speed, arguments are hardcoded here.*/
#define GPIO_PIXEL_DATA (*(volatile unsigned short *)(GPIO_0_BASE_ADDRESS + (PIXEL_PINS << 2)))

#define DRAM_BASE_ADDR 0x80000000 /* Need DRAM to save image data.*/

/** 
 * @fn void generateClock(unsigned char channelNo, unsigned long frequency)
 * @brief  function to generate input clock for OV7670
 * @param[in] unsigned char channelNo
 * @param[in] unsigned long frequency
 * @param[Out] No output parameter 
*/
/* function to generate input clock for OV7670*/
void generateClock(unsigned char channelNo, unsigned long frequency)
{
	unsigned int onOffTime, period;
	period = SYS_CLK_FREQ / frequency;
	if (period < 2)
	{
		printf("ERROR : Cannot generate CLK greater than %d\n", SYS_CLK_FREQ / 2);
		return;
	}
	if ((period % 2) != 0)
	{
		period++;
	}
	onOffTime = period / 2;
	if ((period * frequency) != SYS_CLK_FREQ)
	{
		printf("WARN : CLK Frequency geneteted is %d Hz\n", SYS_CLK_FREQ / period);
	}
	PWMC_Set_Period(channelNo, period);
	PWMC_Set_OnOffTime(channelNo, onOffTime);
	PWMC_init(channelNo, PWM_CONTINUOUS_MODE, PWM_LEFT_ALIGN, PWM_LOC_INTR_DIS, PWM_OPC_LOW, PWM_REPEAT_COUNT);
	PWMC_Enable();
}
typedef struct tm1637Display
{
	unsigned char clkPin;
	unsigned char DioPin;
	unsigned char brightness;
} tm1637;

volatile US *gpioDirAddr1 = (volatile US *)(GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.
volatile US *gpioDirAddr2 = (volatile US *)(GPIO_1_BASE_ADDRESS + 0x40000); // GPIO 1 direction register.

/** 
 * @fn void setWireInput(unsigned char gpioPin)
 * @brief  [HIGH] Sets GPIO pin in input mode.
 * @param[in] unsigned char gpioPin
 * @param[Out] No output parameter 
*/
/* [HIGH] Sets GPIO pin in input mode.*/
void setWireInput(unsigned char gpioPin)
{
	unsigned short bitPos, dirData;

	if (gpioPin > 15)
	{
		bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
		dirData = *gpioDirAddr2;		// Address of the direction register.
		dirData &= ~(bitPos);			// Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr1 = dirData;		// Data written to direction register.
		__asm__ __volatile__("fence");
	}
	else
	{
		bitPos = (1 << gpioPin); // Align the selected pin to its position.
		dirData = *gpioDirAddr1; // Address of the direction register.
		dirData &= ~(bitPos);	 // Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr1 = dirData; // Data written to direction register.
		__asm__ __volatile__("fence");
	}
}
/** 
 * @fn void setWireOutput(unsigned char gpioPin)
 * @brief [LOW] Sets GPIO pin in output mode.
 * @param[in]  unsigned char gpioPin  
 * @param[Out] No output parameter 
*/
/* [LOW] Sets GPIO pin in output mode.*/
void setWireOutput(unsigned char gpioPin)
{
	unsigned short bitPos, dirData;

	if (gpioPin > 15)
	{
		bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
		dirData = *gpioDirAddr2;		// Address of the direction register.
		dirData |= (bitPos);			// Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr1 = dirData;		// Data written to direction register.
		__asm__ __volatile__("fence");
	}
	else
	{
		bitPos = (1 << gpioPin); // Align the selected pin to its position.
		dirData = *gpioDirAddr1; // Address of the direction register.
		dirData |= (bitPos);	 // Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr1 = dirData; // Data written to direction register.
		__asm__ __volatile__("fence");
	}
}
/** 
 * @fn bitDelay
 * @brief Delay for tm1637.
 * @param[in]  No input parameter  
 * @param[Out] No output parameter 
*/
/* Delay for tm1637.*/
void bitDelay(void)
{
	udelay(SIO_CLOCK_DELAY);
}

/** 
 * @fn void setWireData(unsigned char gpioPin, unsigned char data)
 * @brief Sets GPIO pin in output mode.
 * @param[in] unsigned char gpioPin
 * @param[in] unsigned char data
 * @param[Out] No output parameter 
*/

void setWireData(unsigned char gpioPin, unsigned char data)
{
	unsigned short bitPos, *gpioDirAddrPtr;
	unsigned long gpioDirAddrData;

	if (gpioPin > 15)
	{
		gpioPin = gpioPin -16;
		bitPos = (1 << (gpioPin)); // Align the selected pin to its position.
		gpioDirAddrData = GPIO_1_BASE_ADDRESS;
	}
	else
	{
		bitPos = (1 << gpioPin); // Align the selected pin to its position.
		gpioDirAddrData = GPIO_0_BASE_ADDRESS;
	}

	gpioDirAddrData |= (bitPos << 2); // Generate address of the selected GPIO pin in GPIO 0.
	gpioDirAddrPtr = (US *)gpioDirAddrData;
	*gpioDirAddrPtr = (data << gpioPin);
	__asm__ __volatile__("fence");
}
/** 
 * @fn InitSCCB
 * @brief InitSCCB
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void InitSCCB(void) //SCCB Initialization
{
	setWireOutput(SIO_C);
	setWireOutput(SIO_D);

	setWireData(SIO_C, HIGH);
	setWireData(SIO_D, HIGH);

	printf("Init SCCB - Port Direction Set & Set High OK\n");
}
/** 
 * @fn StartSCCB
 * @brief StartSCCB
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void StartSCCB(void) //SCCB Start
{

	setWireData(SIO_D, HIGH);
	bitDelay();

	setWireData(SIO_C, HIGH);
	bitDelay();

	setWireData(SIO_D, LOW);
	bitDelay();

	setWireData(SIO_C, LOW);
	bitDelay();
}
/** 
 * @fn StopSCCB
 * @brief StopSCCB
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void StopSCCB(void) //SCCB Stop
{
	setWireData(SIO_D, LOW);
	bitDelay();

	setWireData(SIO_C, HIGH);
	bitDelay();

	setWireData(SIO_D, HIGH);
	bitDelay();
}
/** 
 * @fn unsigned char SCCBWrite(unsigned char data)
 * @brief SCCBWrite
 * @param[in] unsigned char data
 * @param[Out] No output parameter 
*/
unsigned char SCCBWrite(unsigned char data)
{
	unsigned char j, success;

	for (j = 0; j < 8; j++) //Loop transmit data 8 times
	{
		if ((data << j) & 0x80)
			setWireData(SIO_D, HIGH);
		else
			setWireData(SIO_D, LOW);

		bitDelay();

		setWireData(SIO_C, HIGH);
		bitDelay();

		setWireData(SIO_C, LOW);
		bitDelay();
	}

	setWireInput(SIO_D);	 // Pass a bus of SIO_D to slave (OV7670)
	setWireData(SIO_D, LOW); // Pull-up prevention  --this line is not present in embedded programmer lib
	bitDelay();

	setWireData(SIO_C, HIGH);
	bitDelay();

	if (GPIO_read_pin(SIO_D) == HIGH)
		success = 0;
	else
		success = 1;

	setWireData(SIO_C, LOW);
	bitDelay();

	setWireOutput(SIO_D); //Return the bus of SIO_D to master (Arduino)

	return success;
}
/* Write using SCCB.*/
/** 
 * @fn void unsigned char WriteOV7670(char regID, char regData)
 * @brief Write using SCCB.
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
unsigned char WriteOV7670(char regID, char regData)
{
	StartSCCB();
	if (!SCCBWrite(WRITE_SLAVE_ADDR))
	{
		printf(" Write Error 0x42\n");
		StopSCCB();
		return 0;
	}

	bitDelay();

	if (!SCCBWrite(regID))
	{
		printf(" Write Error ID\n");
		StopSCCB();
		return 0;
	}
	bitDelay();
	if (!SCCBWrite(regData))
	{
		StopSCCB();
		return 0;
	}

	StopSCCB();

	return 1;
}
/* Initializing OV7670 and settting preferred modes.*/
/** 
 * @fn void initOV7670()
 * @brief Initializing OV7670 and settting preferred modes.
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void initOV7670()
{
	volatile unsigned short *gpio0DirAddr = (volatile unsigned short *)(GPIO_0_BASE_ADDRESS + 0x40000);
	unsigned short gpio0DirData;

	generateClock(0, 20000000);

	printf("Initializing OV7670\n");
	InitSCCB();
	printf("Resetting SCCB... ");
	if (!WriteOV7670(0x12, 0x80))
	{
		printf("Failed\n");
	}
	else
	{
		printf("OK\n");
	}
	printf("Setting clock division... ");
	if (!WriteOV7670(0x11, 0x09))
	{
		printf("Failed\n");
	}
	else
	{
		printf("OK\n");
	}
	printf("Enabling scaling... ");
	if (!WriteOV7670(0x0C, 0x08))
	{
		printf("Failed\n");
	}
	else
	{
		printf("OK\n");
	}
	printf("Changing Resolution to QCIF[174 x 144] ... ");
	if (!WriteOV7670(0x12, 0x08))
	{
		printf("Failed\n");
	}
	else
	{
		printf("OK\n");
	}
	printf("Communication Over \n");
	/* Setting pixel data GPIOS (0-10) in input mode */
	gpio0DirData = *gpio0DirAddr;
	gpio0DirData &= ~((unsigned short)PIXEL_PINS);
	*gpio0DirAddr = gpio0DirData;
}
/* Main Function*/
/** 
 * @brief Main function to capture the image frame
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main()
{
	printf("+-------[ OV7670 Camera Module Demo ]-------+\n");
	printf("|                                           |\n");
	printf("|              D0 ---> GPIO0                |\n");
	printf("|              D1 ---> GPIO1                |\n");
	printf("|              D2 ---> GPIO2                |\n");
	printf("|              D3 ---> GPIO3                |\n");
	printf("|              D4 ---> GPIO4                |\n");
	printf("|              D5 ---> GPIO5                |\n");
	printf("|              D6 ---> GPIO6                |\n");
	printf("|              D7 ---> GPIO7                |\n");
	printf("|                                           |\n");
	printf("|            PCLK ---> GPIO8                |\n");
	printf("|           HSYNC ---> GPIO9                |\n");
	printf("|           vSYNC ---> GPIO10               |\n");
	printf("|                                           |\n");
	printf("|           MCLCK ---> PWM0                 |\n");
	printf("|                                           |\n");
	printf("|             SCL ---> GPIO11               |\n");
	printf("|             SDA ---> GPIO12               |\n");
	printf("+-------------------------------------------+\n\n");

	udelay(100);
	initOV7670();
	
	int index = 0,count;
	volatile unsigned short pixelData;
	volatile unsigned char * pixelArray = (unsigned char *) DRAM_BASE_ADDR ;

	/* Waiting for new frame.*/
	while (1){
		if(GPIO_PIXEL_DATA & VSYNC){
			break;
		}
	}
	count = 0;
	/* Capturing frame.*/
	while (1)
	{
		pixelData = GPIO_PIXEL_DATA;
		if((pixelData & PCLK) && (pixelData & HSYNC)){
			* pixelArray = (unsigned char) (pixelData);
			pixelArray ++;
			count ++;
			while(GPIO_PIXEL_DATA& PCLK);
		}
		else if((pixelData & VSYNC)&& count)
			break;
	}
	printf("Bytes read : %d\n",count);
	printf("---------------------------------[ Grayscale image data starts here ]---------------------------------\n\n");
	pixelArray = (unsigned char *) DRAM_BASE_ADDR;
	/* Sending frame [only sending grayscale part] via UART ie We remove Cr and Cb parts.*/
	for ( index = 0,pixelArray++; index < PIXELS_PER_FRAME; index++, pixelArray+=2 )
	{
		printf("%d ", *pixelArray);
		if (((index +1 ) % 20) == 0)
			printf("\n");
	}
	printf("\n\n----------------------------------[ Grayscale image data ends here ]----------------------------------\n");
	while (1)
		;
}
