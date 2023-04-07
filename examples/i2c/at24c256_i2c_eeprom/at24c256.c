/***************************************************************************
* Module name                           :  at24c256.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  Library for at24c256 i2c eeprom

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

#include "i2c_api.h"
#include "at24c256.h"
#include "stdlib.h"

void at24c256Begin(unsigned char i2cNo)
{
	i2c_init(i2cNo);
}

// Function to write to EEPROOM
void at24c256Write(char *writeBuf, int length, unsigned short eepromAddress)
{
	i2c_beginTransmission(AT24C256_EEPROM_I2C_ADDRESS);

	i2c_write(eepromAddress >> 8);
	i2c_write(eepromAddress & 0xFF);
	while (length)
	{
		i2c_write(*writeBuf);
		length--;
		writeBuf++;
	}
	i2c_endTransmission(TRUE);
	udelay(500);
}

// Function to read from EEPROM
void at24c256Read(char *readBuf, int length, unsigned short eepromAddress)
{
	i2c_beginTransmission(AT24C256_EEPROM_I2C_ADDRESS);
	i2c_write(eepromAddress >> 8);
	i2c_write(eepromAddress & 0xFF);
	i2c_endTransmission(TRUE);

	//delay(5);
	//udelay(5000);
	i2c_requestFrom(AT24C256_EEPROM_I2C_ADDRESS, length, TRUE);
	//delay(1);
	while (length)
	{
		*readBuf = i2c_read(); //data
		readBuf++;
		length--; // since 2 bytes are read in one time.
	}
}