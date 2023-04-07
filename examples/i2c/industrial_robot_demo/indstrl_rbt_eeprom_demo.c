/***************************************************************************
* Module name                           :  indstrl_rbt_eeprom_demo.c
* Name of Author                        :  thomas sebastian
* Email ID  (Report any bugs)           :  vega@cdac.in
* Module Description                    :  indstrl_rbt_eeprom_demo program

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
#include "adc.h"
#include "config.h"
#include "servo.h"
#include "debug_uart.h"
#include "ads1015.h"


/**
 @brief read ADC data in A1 from the pot .
 @details reads ADC values and stores in the eeprom 24c256.
 @param[in] No input parameters.
 @param[Out] No ouput parameter.
 @return Void function.
 */
 
 void read_data();
 void generate_data();
 void servo_data();
 void write_data();
 
 
  static char writeData[512], readData[512];
  int stride = 0;

void main()
{
  

  printf("+----------------------[ AT24C256 Demo ]--------------------+\n\n");
  printf("+                                       			 +\n\n");
  printf("+ 			connect pot pin to A0			 +\n\n");
  printf("+			connect SERVO pin to IO5		 +\n\n");
  printf("+		connect EEPROM pin to I2C 1 (SCL &SDA)		 +\n\n");
  printf("+                                       			 +\n\n");
  printf("+-----------------------------------------------------------+\n\n");
  
  
  
  

   // generate_data();
	  read_data();   		//initial read from the eeprom 24c256 .
	  generate_data();		//reading the data from the pot && live servo action according to pot(ADC) value.
	  write_data();		//store the new data to the eeprom 24c256.
	  read_data();			//read the updated data from the eeprom 24c256.
	  servo_data();		//servo action w.r.t the updated data from eeprom 24c256.
	   while (1);
	   
}

void read_data()
{
  at24c256Begin(I2C_1);
// reading existing data.
  stride = 0;
  while (stride < 512)
  {
    at24c256Read(readData + stride, 16, stride); // Note : Max read length supported is 16.
    stride += 16;
  }
  printf("\n at24c256 read data :");
  
  int result_volt, *tempReadPrt = (int *)readData;
  
  for (int i = 0; i < 30; i++) // reading 30 values.
  {
    if (i % 10 == 0) // single row contain 10 values.
     printf("\n");
    printf("%5d ", tempReadPrt[i]);
    
     
  }
  printf("\n");

}

void generate_data()
{

  ads1015Begin(I2C_ADC);

 // generating data.
 float resolution = 0.002; 
  	US adc_data = 0,count=0;
	int result_volt, *tempWritePrt = (int *)writeData; 
	printf("\n\r Reading ADC Channel 0 data\n\r");
	
	 for (int i = 0; i < 30; i++)
 	 {
      	printf("\n Reading ADC Channel 0 data %d\n",i);
 	 	rx_uart();					// in order to make servo action step by step .... replace if needed with delay fn....
      	printf("dout");
	  	adc_data =  ads1015ReadSingleEnded(0); //(A0)
	  	result_volt = adc_data ;
	  	
	  	printf ("\n A0 data: %d ", result_volt);
	  	//udelay(7000);
	  	
	  	tempWritePrt[i] = result_volt;		// storing to a temp pointer
	  	servoRotate(PWM_CH_0,result_volt);		// servo action according to A1 value
  	}
  
  
}

void servo_data()
{
  at24c256Begin(I2C_1);

// reading existing data.
int result_volt, *tempReadPrt = (int *) readData; 
  stride = 0;
  while (stride < 512)
  {
    at24c256Read(readData + stride, 16, stride); // Note : Max read length supported is 16.
    stride += 16;
  }

	  for (int i = 0; i < 30; i++)
	  {
	   result_volt=tempReadPrt[i];				// reading from the temp pointer
	   printf ("\n press any key to Rotate : %6d",result_volt);   	
	   servoRotate(PWM_CH_0,result_volt);				// servo action according to stored EEPROM  value
	   rx_uart();  				// in order to make servo action step by step .... replace if needed with appropriate delay fn....
	  }
 
}

void write_data()
{
  at24c256Begin(I2C_1);
  // setting up and writing data.
  stride = 0;
  while (stride < 512)
  {
    at24c256Write(writeData + stride, 64, stride); // Note : Max write length supported is 128, but 64 is safe for calculation.
    stride += 64;
  }
}
