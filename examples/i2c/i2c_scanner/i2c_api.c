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

#include "i2c.h"
#include "stdlib.h"

unsigned char txAddress=0, i2c_number=0;

void i2c_init(unsigned char i2c_num){
	i2c_number=i2c_num;
	i2c_configure(i2c_number, 100000000, 100000);
}


void i2c_beginTransmission(unsigned char i2c_address) {
	i2c_start(i2c_number,0,0);
	txAddress=i2c_address;
}

unsigned char i2c_endTransmission() {
	unsigned char ret = i2c_data_write(i2c_number, &txAddress, 1);
	i2c_stop(i2c_number);
	return ret;
}



