

/***************************************************
* Module name: lcd.c
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
*
***************************************************/

/*  Include section
*
*
***************************************************/
/**
 @file lcd.c
 @brief Contains routines to display text in 20*4 LCD display
 */
#include "stdlib.h"
#include "config.h"
#include "lcd.h"
#include "gpio.h"


#define RS_PIN		PIN_3
#define RW_PIN		PIN_4
#define ENABLE_PIN	PIN_5

const char dataPins[]={PIN_6,PIN_7,PIN_8,PIN_9};


volatile US *gpioDirAddr1 = (volatile US *)(GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.
volatile US *gpioDirAddr2 = (volatile US *)(GPIO_1_BASE_ADDRESS + 0x40000); // GPIO 1 direction register.

unsigned char  displayFunction = 0, displayControl = 0, displayMode = 0;
uint8_t rowOffsets[4],numlines;

/* Sets GPIO pin in input mode.*/
void setWireInput(unsigned char gpioPin)
{
  unsigned short bitPos, dirData;

  if (gpioPin > 15)
  {
    bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
    dirData = *gpioDirAddr2;        // Address of the direction register.
    dirData &= ~(bitPos);           // Clearing a bit configures the pin to be INPUT.
    *gpioDirAddr2 = dirData;        // Data written to direction register.
    __asm__ __volatile__("fence");
  }
  else
  {
    bitPos = (1 << gpioPin); // Align the selected pin to its position.
    dirData = *gpioDirAddr1; // Address of the direction register.
    dirData &= ~(bitPos);    // Clearing a bit configures the pin to be INPUT.
    *gpioDirAddr1 = dirData; // Data written to direction register.
    __asm__ __volatile__("fence");
  }
}

/* Sets GPIO pin in output mode.*/
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

/* Sets GPIO pin in output mode.*/
void setGpioPin(unsigned char gpioPin, unsigned char data)
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

   gpioDirAddrData |= (bitPos << 2);		// Generate address of the selected GPIO pin in GPIO 0.
   gpioDirAddrPtr = (US *)gpioDirAddrData;
   *gpioDirAddrPtr = (data << gpioPin);
    __asm__ __volatile__("fence");
}


void pulseEnable(void) {
  setGpioPin(ENABLE_PIN, LOW);
  udelay(1);    
  setGpioPin(ENABLE_PIN, HIGH);
  udelay(1);    // enable pulse must be >450ns
  setGpioPin(ENABLE_PIN, LOW);
  udelay(100);   // commands need > 37us to settle
}

void write4bits(uint8_t value) {
  for (int i = 0; i < 4; i++) {
    setGpioPin(dataPins[i], (value >> i) & 0x01);
  }
  
  pulseEnable();
}

void sendCommand(uint8_t value) {
  setGpioPin(RS_PIN, LOW);
  write4bits(value>>4);
  write4bits(value);
}

size_t sendData(uint8_t value) {
  setGpioPin(RS_PIN, HIGH);
  write4bits(value>>4);
  write4bits(value);
  return 1; // assume sucess
}

void display() {
  displayControl |= LCD_DISPLAYON;
  sendCommand(LCD_DISPLAYCONTROL | displayControl);
}

void clear()
{
  sendCommand(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  udelay(2000);  // this command takes a long time!
}

void setRowOffsets(int row0, int row1, int row2, int row3)
{
  rowOffsets[0] = row0;
  rowOffsets[1] = row1;
  rowOffsets[2] = row2;
  rowOffsets[3] = row3;
}

void setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(rowOffsets) / sizeof(*rowOffsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= numlines ) {
    row = numlines - 1;    // we count rows starting w/0
  }
  
  sendCommand(LCD_SETDDRAMADDR | (col + rowOffsets[row]));
}

/** @fn lcdInit
 * @brief Initialize LCD display
 * @details 
 * @warning 
 * @param[in] No input parameter 
 * @param[Out] No output parameter 
*/
void lcdInit(void)
{
	numlines = 4;

	displayFunction = LCD_2LINE | LCD_4BITMODE | LCD_5x8DOTS;

	setRowOffsets(0x00, 0x40, 0x00 + NUM_COLUMNS, 0x40 + NUM_COLUMNS); 

	setWireOutput(ENABLE_PIN);	
	setWireOutput(RS_PIN);	
	setWireOutput(RW_PIN);	

	for(int i=0;i<4;i++)
		setWireOutput(dataPins[i]);

	udelay(5000);	

	setGpioPin(ENABLE_PIN,LOW);
	setGpioPin(RS_PIN,LOW);
	setGpioPin(RW_PIN,LOW);
	
	

	write4bits(0x03);
	udelay(4500); // wait min 4.1ms

	// second try
	write4bits(0x03);
	udelay(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03); 
	udelay(150);

	// finally, set to 4-bit interface
	write4bits(0x02); 


	sendCommand(LCD_FUNCTIONSET | displayFunction);

	// turn the display on with no cursor or blinking default
	displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for romance languages)
	displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	sendCommand(LCD_ENTRYMODESET | displayMode);
}

/** @fn lcdPrint
 * @brief To print string in LCD
 * @details 
 * @warning 
 * @param[in] String to be print
 * @param[Out] No output parameter 
*/
void lcdPrint(char * str)
{
	while(*str!='\0'){
		sendData(*str);
		str++;
	}
}


