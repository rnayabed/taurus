/***************************************************************************
* Module name                           :  ads1015_demo.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  ads1015 demo program

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

/**
 @brief read ADC data in ads1015.
 @details reads ads1015 ADC values with I2C interface.
 @param[in] No input parameters.
 @param[Out] No ouput parameter.
 @return Void function.
 */
void main()
{
  float resolution = 0.002;
  float adc_array[4];        
  	printf("+----------------------[ ADS1015 Demo ]--------------------+\n\n");
  ads1015Begin(I2C_ADC);
  while (1)
  {
    for (int i = 0; i < 4; i++)
    {
      adc_array[i] = resolution * ads1015ReadSingleEnded(i);
    }
    udelay(50000); // Wait 5 seconds for next scan
    printf("\r adc0 : %0.2fV   adc1 : %0.2fV   adc2 : %0.2fV   adc3 : %0.2fV", adc_array[0], adc_array[1], adc_array[2], adc_array[3]);
  }
}
