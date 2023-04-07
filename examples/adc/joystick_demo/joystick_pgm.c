/***************************************************************************
* Module name                           :  joystick_pgm.c
* Name of Author                        :  Maitreya, C-DAC
* Email ID  (Report any bugs)           :  ramakant.km@cdac.in
* Module Description                    :  Joystick demo program

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
#include "gpio.h"
#include "config.h"

#define VRX_PIN   0 //Connect to the Analog channel A0 of the ARIES Board
#define VRY_PIN   1 //Connect to the Analog channel A1 of the ARIES Board
#define SW_PIN   2  //Switch pin,connected to the digital pin i.e. to GPIO2 of the ARIES Board
/**
 @brief read ADC data in ads1015.
 @details reads ads1015 ADC values with I2C interface.
 @param[in] No input parameters.
 @param[Out] No ouput parameter.
 @return Void function.
 */
void main()
{
  int Xval=0,Yval = 0,bval;       
  	printf("+----------------------[ JOYSTICK Demo ]--------------------+\n\n");
  ads1015Begin(I2C_ADC);
  while (1)
  {
      Xval = ads1015ReadSingleEnded(VRX_PIN);//outputs an analog value corresponding to the horizontal position
      Yval = ads1015ReadSingleEnded(VRY_PIN);//outputs an analog value corresponding to the vertical position
      GPIO_write_pin(SW_PIN,HIGH);
      bval = GPIO_read_pin(SW_PIN);
      
    udelay(50000); // Wait 5 seconds for next scan
    printf("Xvalue = %d\n",Xval);
    printf("Yvalue = %d\n",Yval);
    printf("bval = %d\n" , bval);

    }
  }
