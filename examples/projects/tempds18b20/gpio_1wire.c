/***************************************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  DS18B20 Temperature Sensor Demo

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

#include "stdlib.h"
#include "ds18b20.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

/**
 @file gpio_1wire.c
 @brief Contains routines for read temperature using 1-wire protocol
 */

/**
  @brief To read temperature using 1-wire protocol
  @details 
  @warning 
  @param[in] No input parameter 
  @param[Out] No output parameter 
*/

void main()
{
	int temp;
	float celcius, sign;
	while (1)
	{
		temp = getRawTemperature();
		if (temp == DS18B20_NOT_FOUND)
		{
			printf("DS18B20 Sensor not found at GPIO %d", ONE_WIRE_GPIO);
		}
		else
		{
			sign = 1.0;
			if(temp & TEMP_SIGN_BIT){
				sign = -1.0;
				temp = (temp & (~TEMP_SIGN_BIT));
			}
			celcius = sign * ((float)temp / 16.0);
			printf("Calculated Temp : %f Celcius\n", celcius);
		}
		for (int i = 0; i < 2060000; i++) // 1 sec delay
			delay_1_us();
	}
}
