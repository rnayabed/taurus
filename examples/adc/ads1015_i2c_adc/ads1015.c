/***************************************************************************
* Module name                           :  ads1015.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  Library for ads1015

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

#include "i2c_api.h"
#include "ads1015.h"
#include "stdlib.h"

#define TRUE 1
#define FALSE 0

/**
 @file ads1015.c
 @brief Contains routines to display text in 20*4 LCD display
 */

/** @fn ads1015Begin
 * @brief Initialize ads1015
 * @details 
 * @param[in] int i2c number for communication
 * @param[Out] No output parameter 
*/

void ads1015Begin(unsigned char i2c_num)
{
	i2c_init(i2c_num);
}

static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value)
{
	i2c_beginTransmission(i2cAddress);
	i2c_write((uint8_t)reg);
	i2c_write((uint8_t)(value >> 8));
	i2c_write((uint8_t)(value & 0xFF));
	i2c_endTransmission(TRUE);
}

static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
	i2c_beginTransmission(i2cAddress);
	i2c_write(reg);
	i2c_endTransmission(TRUE);
	i2c_requestFrom(i2cAddress, (uint8_t)2, TRUE);
	return ((i2c_read() << 8) | i2c_read());
}

/** @fn ads1015ReadSingleEnded
 * @brief read a ADC channel 12 bit data from ads1015
 * @details 
 * @param[in] int ADC channel number
 * @param[Out] No output parameter 
*/

uint16_t ads1015ReadSingleEnded(uint8_t channel)
{
	if (channel > 3)
	{
		printf("invalid channel %d !\n", channel);
		return 0;
	}

	// Start with default values
	uint16_t config =
		ADS1015_REG_CONFIG_CQUE_NONE |	  // Disable the comparator (default val)
		ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
		ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
		ADS1015_REG_CONFIG_CMODE_TRAD |	  // Traditional comparator (default val)
		ADS1015_REG_CONFIG_DR_3300SPS |	  // 3300 samples per second
		ADS1015_REG_CONFIG_MODE_SINGLE;	  // Single-shot mode (default)

	// Set PGA/voltage range
	config |= ADS1015_REG_CONFIG_PGA_4_096V;

	// Set single-ended input channel
	switch (channel)
	{
	case (0):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
		break;
	case (1):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
		break;
	case (2):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
		break;
	case (3):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
		break;
	}

	// Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	writeRegister(ADS1015_ADDRESS, ADS1015_REG_POINTER_CONFIG, config);

	// Wait for the conversion to complete
	udelay(ADS1015_CONVERSIONDELAY);

	// Read the conversion results
	// Shift 12-bit results right 4 bits for the ADS1015
	return readRegister(ADS1015_ADDRESS, ADS1015_REG_POINTER_CONVERT) >> BIT_SHIFT;
}
