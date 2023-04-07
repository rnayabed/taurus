/***************************************************************************
* Module name                           :  temperature_monitor.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
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
	printf("+------[ COVID19 Temperature Monitor SYSTEM ]------+\n");
	printf("|                                                  |\n");
	printf("|                 SCL ---> GPIO6                   |\n");
	printf("|                 SDA ---> GPIO5                   |\n");
	printf("|              Buzzer ---> GPIO0                   |\n");
	printf("|                                                  |\n");
	printf("+--------------------------------------------------+\n\n");
	mlx90614 myTempSensor;
	double ambientTemp, targetTemp;

	myTempSensor.sclPin = PIN_6;
	myTempSensor.sdaPin = PIN_5;
	GPIO_write_pin(PIN_0, LOW);
	while (1)
	{
		ambientTemp = mlx90614ReadAmbientTempC(myTempSensor);
		targetTemp = mlx90614ReadTargetTempC(myTempSensor);
		printf("\r[INFO] Target Temp : %8.3f *C", targetTemp);
		if (targetTemp > 33)
		{
			GPIO_write_pin(PIN_0, HIGH);
			if (targetTemp > 37)
			{
				udelay(50000);
			}
			udelay(4000);
			GPIO_write_pin(PIN_0, LOW);
			udelay(300000);
		}
		udelay(5000);
	}
}
