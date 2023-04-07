/***************************************************************************
* Module name                           :  at24c256_demo.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  at24c256 i2c demo program

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

/**
 @brief read ADC data in ads1015.
 @details reads ads1015 ADC values with I2C interface.
 @param[in] No input parameters.
 @param[Out] No ouput parameter.
 @return Void function.
 */
void main()
{
  static char writeData[512], readData[512];
  int stride = 0;

  printf("+----------------------[ AT24C256 Demo ]--------------------+\n\n");
  at24c256Begin(I2C_0);

  // reading existing data.
  stride = 0;
  while (stride < 512)
  {
    at24c256Read(readData + stride, 16, stride); // Note : Max read length supported is 16.
    stride += 16;
  }
  printf("\n at24c256 read data :");
  for (int i = 0; i < 512; i++)
  {
    if (i % 32 == 0)
      printf("\n");
    printf("%02x ", readData[i]);
  }
  printf("\n");

  // generating data.
  for (int i = 0; i < 512; i++)
  {
    writeData[i] = i % 256;
  }
  // setting up and writing data.
  stride = 0;
  while (stride < 512)
  {
    at24c256Write(writeData + stride, 64, stride); // Note : Max write length supported is 128, but 64 is safe for calculation.
    stride += 64;
  }

  // reading new data.
  stride = 0;
  while (stride < 512)
  {
    at24c256Read(readData + stride, 16, stride); // Note : Max read length supported is 16.
    stride += 16;
  }
  printf("\n at24c256 read data :");
  for (int i = 0; i < 512; i++)
  {
    if (i % 32 == 0)
      printf("\n");
    printf("%02x ", readData[i]);
  }
  printf("\n");

  while (1)
    ;
}
