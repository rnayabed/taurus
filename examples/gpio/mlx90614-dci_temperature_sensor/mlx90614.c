/***************************************************************************
* Module name                           :  mlx90614.c
* Name of Author                        :  Siva Prasad S, C-DAC
* Email ID  (Report any bugs)           :  sivaprasad@cdac.in
* Module Description                    :  Library for mlx90614 Temperature sensor.

  Copyright (C) 2021  CDAC(T). All rights reserved.

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
#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include "mlx90614.h"

volatile US *gpioDirAddr1 = (volatile US *)(GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.
volatile US *gpioDirAddr2 = (volatile US *)(GPIO_1_BASE_ADDRESS + 0x40000); // GPIO 1 direction register.

void delay_250u(void)
{
	usleep(250);
	
}

void delay_10u(void)
{

usleep(10);
	
}

void delay_5u()

{
	usleep(5);
	
}
/* Sets GPIO pin in input mode.*/
void setWireInput(unsigned char gpioPin)
{
	unsigned short bitPos, dirData;

	if (gpioPin > 15)
	{
		bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
		dirData = *gpioDirAddr2;		// Address of the direction register.
		dirData &= ~(bitPos);			// Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr2 = dirData;		// Data written to direction register.
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

/* Sets GPIO pin in output mode.*/
void setGpioPin(unsigned char gpioPin, unsigned char data)
{
	unsigned short bitPos, *gpioDirAddrPtr;
	unsigned long gpioDirAddrData;

	if (gpioPin > 15)
	{
		gpioPin = gpioPin - 16;
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

void mlx90614Start(mlx90614 tempSensor)
{
	setWireOutput(tempSensor.sclPin);
	setWireOutput(tempSensor.sdaPin);

	setGpioPin(tempSensor.sdaPin, HIGH);
	setGpioPin(tempSensor.sclPin, HIGH);
	delay_250u();
	setGpioPin(tempSensor.sdaPin, LOW);
	delay_5u();
	setGpioPin(tempSensor.sclPin, LOW);
}

void mlx90614Stop(mlx90614 tempSensor)
{
	setWireOutput(tempSensor.sclPin);
	setWireOutput(tempSensor.sdaPin);

	setGpioPin(tempSensor.sdaPin, LOW);
	setGpioPin(tempSensor.sclPin, HIGH);
	delay_10u();
	delay_10u();
	setGpioPin(tempSensor.sdaPin, HIGH);
	delay_10u();
}

void mlx90614Restart(mlx90614 tempSensor)
{
	setWireOutput(tempSensor.sclPin);
	setWireOutput(tempSensor.sdaPin);

	setGpioPin(tempSensor.sdaPin, LOW);
	setGpioPin(tempSensor.sclPin, LOW);
	delay_5u();
	setGpioPin(tempSensor.sdaPin, HIGH);
	delay_5u();
	setGpioPin(tempSensor.sclPin, HIGH);
	delay_5u();
	setGpioPin(tempSensor.sdaPin, LOW);
	delay_5u();
	setGpioPin(tempSensor.sclPin, LOW);
}

void writebyte(mlx90614 tempSensor, UC data)
{
	setWireOutput(tempSensor.sclPin);
	setWireOutput(tempSensor.sdaPin);

	setGpioPin(tempSensor.sdaPin, LOW);
	setGpioPin(tempSensor.sclPin, LOW);

	for (int i = 0; i < 8; i++)
	{
		//delay_5u();
		if (data & 0x80)
		{
			setGpioPin(tempSensor.sdaPin, HIGH);
		}
		else
		{
			setGpioPin(tempSensor.sdaPin, LOW);
		}
		data = data << 1;
		delay_5u();
		setGpioPin(tempSensor.sclPin, HIGH);
		delay_5u();
		setGpioPin(tempSensor.sclPin, LOW);
	}
	//ACK = GPIO_read_pin(SDA);
	setWireInput(tempSensor.sdaPin);
	delay_5u();
	setGpioPin(tempSensor.sclPin, HIGH);
	delay_5u();
	setGpioPin(tempSensor.sclPin, LOW);
}
unsigned char readbyte(mlx90614 tempSensor)
{
	unsigned char data = 0, bit;
	setWireOutput(tempSensor.sclPin);
	setWireInput(tempSensor.sdaPin);

	//setGpioPin(tempSensor.sdaPin, HIGH);
	setGpioPin(tempSensor.sclPin, LOW);
	
	delay_5u();
	for (int i = 0; i < 8; i++)
	{
		delay_5u();
		setGpioPin(tempSensor.sclPin, HIGH);
		delay_5u();
		bit = GPIO_read_pin(tempSensor.sdaPin);
		setGpioPin(tempSensor.sclPin, LOW);
		data = data | (bit << (7 - i));
	}
	//ack
	setWireOutput(tempSensor.sdaPin);
	setGpioPin(tempSensor.sdaPin, LOW);
	delay_5u();
	setGpioPin(tempSensor.sclPin, HIGH);
	delay_5u();
	setGpioPin(tempSensor.sclPin, LOW);
	return data;
}
double mlx90614ReadTempC(mlx90614 tempSensor, unsigned char reg)
{
	double tempC;
	unsigned int tempData = 0;
	mlx90614Start(tempSensor);
	writebyte(tempSensor, MLX90614_WRITE_ADDR);
	writebyte(tempSensor, reg);
	mlx90614Restart(tempSensor);
	writebyte(tempSensor, MLX90614_READ_ADDR);
	tempData = readbyte(tempSensor);
	tempData |= readbyte(tempSensor) << 8;
	mlx90614Stop(tempSensor);
	tempC = tempData * 0.02;
	tempC -= 273.15;
	return tempC;
}

double mlx90614ReadAmbientTempC(mlx90614 tempSensor)
{
	return mlx90614ReadTempC(tempSensor, MLX90614_AMBIENT_TEMP);
}

double mlx90614ReadTargetTempC(mlx90614 tempSensor)
{
	return mlx90614ReadTempC(tempSensor, MLX90614_TARGET_TEMP);
}