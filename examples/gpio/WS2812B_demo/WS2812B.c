#include "stdlib.h"
#include "gpio.h"
#include "encoding.h"
#include "WS2812B.h"

#define TIME_0_HIGH 40
#define TIME_0_LOW 85
#define TIME_1_HIGH 80
#define TIME_1_LOW 45
#define TIME_RESET 500

volatile US *gpioDirAddr1 = (volatile US *)(GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.
volatile US *gpioDirAddr2 = (volatile US *)(GPIO_1_BASE_ADDRESS + 0x40000); // GPIO 1 direction register.

unsigned char currentGpio;
unsigned int currentLEDs;
unsigned short *currentGpioDirAddrPtr;

/* Sets GPIO pin in output mode.*/
void setWireOutput(unsigned char gpioPin)
{
	unsigned short bitPos, dirData;

	if (gpioPin > 15)
	{
		bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
		dirData = *gpioDirAddr2;		// Address of the direction register.
		dirData |= (bitPos);			// Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr2 = dirData;		// Data written to direction register.
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

void beginWS2812B(unsigned int gpioPin, unsigned int LEDs)
{
	unsigned short bitPos;
	unsigned long gpioDirAddrData;

	currentGpio = gpioPin;
	currentLEDs = LEDs;
	setWireOutput(currentGpio);

	if (currentGpio > 15)
	{
		currentGpio = currentGpio - 16;
		bitPos = (1 << (currentGpio)); // Align the selected pin to its position.
		gpioDirAddrData = GPIO_1_BASE_ADDRESS;
	}
	else
	{
		bitPos = (1 << currentGpio); // Align the selected pin to its position.
		gpioDirAddrData = GPIO_0_BASE_ADDRESS;
	}

	gpioDirAddrData |= (bitPos << 2); // Generate address of the selected GPIO pin in GPIO 0.
	currentGpioDirAddrPtr = (US *)gpioDirAddrData;
}

void setWS2812B(int colorData)
{
	int colorBits, numLEDs;
	numLEDs = currentLEDs;
	while (numLEDs)
	{
		colorBits = WS2812B_COLOR_BITS;
		do
		{
			colorBits--;
			if (colorData & (1 << colorBits))
			{
				/* .8 us delay */
				*currentGpioDirAddrPtr = PIN_DATA_1;
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				*currentGpioDirAddrPtr = PIN_DATA_0;
				/* .45 us delay */
			}
			else
			{
				*currentGpioDirAddrPtr = PIN_DATA_1;
				/* .4 us delay */
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				*currentGpioDirAddrPtr = PIN_DATA_0;
				/* .85 us delay */
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
				__asm__ __volatile__("nop");
			}
		}while (colorBits);
		numLEDs--;
	}
}
