/**
 @file led_breathing.c
 @brief Main program to check the status of LDR sensor
*/

/*********************************************************

 NOTE :- Here Code is optimized for Just LED Controlling.

**********************************************************/
#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

#define RED1 22
#define RED2 19

#define GREEN1 23
#define GREEN2 20

#define BLUE1 24
#define BLUE2 21

#define LED0 16
#define LED1 17
#define LED2 18

#define INHALE_DELAY 400
#define EXHALE_DELAY 520
#define LED_PINS 0x1FF

#define MAX_LED_COMB 7

unsigned short ledCombData[MAX_LED_COMB] = {0x16F, 0x4F, 0xDF, 0x97, 0x1B7, 0x127, 0x1FF};

volatile unsigned short *gpioDirAddr1 = (volatile unsigned short *)(GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.
volatile unsigned short *gpioDirAddr2 = (volatile unsigned short *)(GPIO_1_BASE_ADDRESS + 0x40000); // GPIO 1 direction register.

/* Sets GPIO pin in output mode.*/
/** 
 * @fn void setWireOutput(unsigned char gpioPin)
 * @brief Sets GPIO pin in output mode.
 * @details 
 * @param[in] unsigned char gpioPin
 * @param[Out] No output parameter 
*/
void setWireOutput(unsigned char gpioPin)
{
  unsigned short bitPos, dirData;

  if (gpioPin > 15)
  {
    bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
    dirData = *gpioDirAddr2;        // Address of the direction register.
    dirData |= (bitPos);            // Clearing a bit configures the pin to be INPUT.
    *gpioDirAddr2 = dirData;        // Data written to direction register.
    __asm__ __volatile__("fence");
  }
  else
  {
    bitPos = (1 << gpioPin); // Align the selected pin to its position.
    dirData = *gpioDirAddr1; // Address of the direction register.
    dirData |= (bitPos);     // Clearing a bit configures the pin to be INPUT.
    *gpioDirAddr1 = dirData; // Data written to direction register.
    __asm__ __volatile__("fence");
  }
}

/* Sets GPIO pin [Here 16 31 ] data.*/
/** 
 * @fn void setGpioData(unsigned short data)
 * @brief Sets GPIO pin [Here 16 31 ] data
 * @details 
 * @param[in] unsigned short data
 * @param[Out] No output parameter 
*/
void setGpioData(unsigned short data)
{
  volatile unsigned short * ledPtr= (volatile unsigned short *)(GPIO_1_BASE_ADDRESS + (LED_PINS << 2));
  * ledPtr = data; 
    __asm__ __volatile__("fence");
}
/** 
 * @fn initLeds
 * @brief initialize all LED's
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void initLeds(void){
	setWireOutput(RED1);
	setWireOutput(RED2);
	setWireOutput(GREEN1);
	setWireOutput(GREEN2);
	setWireOutput(BLUE1);
	setWireOutput(BLUE2);
  setWireOutput(LED0);
  setWireOutput(LED1);
  setWireOutput(LED2);
}
/** 
 * @brief To toggle the LED in breathing effect
 * @details 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void main ()
{	
  unsigned long count;
  unsigned short ledData;
	printf("LED Breathing Demo\n");
	initLeds();
  count = 0;
	while (1)
	{
    ledData = ledCombData[count % MAX_LED_COMB];
    count++;
    for(int delay = 0; delay < INHALE_DELAY; delay++){
      setGpioData(ledData);
      udelay(delay);
      setGpioData(0);
			udelay(INHALE_DELAY - delay);
		}
		for(int delay = 0; delay < EXHALE_DELAY; delay++){
      setGpioData(ledData);
			udelay(EXHALE_DELAY - delay);
      setGpioData(0);
			udelay(delay);
		}
	}
}




