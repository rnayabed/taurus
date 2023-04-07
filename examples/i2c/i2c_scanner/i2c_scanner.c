/***************************************************


 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Jul-2020
 Filename		: i2c_scanner.c
 Purpose		: I2C scanner program
 Description		: i2c_scanner program to detect connected I2C device address
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file i2c_scanner.c
 @brief contains routines for I2C scanner
 @detail Includes software functions declarations to initialize,

 */

#include "i2c_api.h"
#include "stdlib.h"

/**
 @fn main
 @brief writes and reads scanner with I2C interface
 @details Initializes configures I2C to write, read and compare data in scanner
 @param[in] No input parameters.
 @param[Out] No ouput parameter.
 @return Void function.

 */



void main() {

	printf("I2C Scanner\n\r");
	i2c_init(I2C_0);
	while(1)
	{
		 int nDevices = 0;
		 printf("Scanning ...\n\r");
		
		 for (int address = 0x3c; address < 0x3e; ++address) {

			    // The i2c_scanner uses the return value of
			    // the Write.endTransmisstion to see if
			    // a device did acknowledge to the address.

			    i2c_beginTransmission(address<<1);
			    int error = i2c_endTransmission();

			    if (error == 0) {
			      printf("I2C device found at address 0x%0x !\n",address);
			      ++nDevices;
			    } 
		 }

		  if (nDevices == 0) {
		    printf("No I2C devices found\n");
		  } else {
		    printf("done\n");
		  }
		  udelay(50000); // Wait 5 seconds for next scan
	}

}

