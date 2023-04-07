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

void main(void)
{
	printf("+-----[ mlx90614 dci Temperature Sensor Demo ]-----+\n");
	printf("|                                                  |\n");
	printf("|                 SCL ---> GPIO6                   |\n");
	printf("|                 SDA ---> GPIO5                   |\n");
	printf("|                                                  |\n");
	printf("+--------------------------------------------------+\n\n");
	mlx90614 myTempSensor;
	double temperature;

	myTempSensor.sclPin = PIN_6;
	myTempSensor.sdaPin = PIN_5;

	while (1)
	{
		temperature = mlx90614ReadAmbientTempC(myTempSensor);
		printf("\r Ambient Temp : %8.3f *C,", temperature);
		temperature = mlx90614ReadTargetTempC(myTempSensor);
		printf(" Target Temp : %8.3f *C", temperature);
		udelay(5000);
	}
}
