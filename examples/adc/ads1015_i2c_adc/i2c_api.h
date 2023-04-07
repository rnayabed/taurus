/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 06-Nov-2020
 Filename		: i2c_api.h
 Purpose		: I2C high level api for i2c programming
 Description	: I2C high level api for i2c programming
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/
/***************************************************/

/*  Include section
*
*
***************************************************/
#ifndef I2C_API_H_
#define I2C_API_H_

#include "stdlib.h"
#include "i2c.h"

#define BUFFER_LENGTH	128

void i2c_init(unsigned char i2c_num);
void i2c_beginTransmission(unsigned char i2c_address);
int i2c_write(unsigned char data);
int i2c_multi_write(unsigned char *data,int length);
char i2c_read(void);
int i2c_requestFrom(int address, int size,unsigned char sendstop);
unsigned char i2c_endTransmission(unsigned char sendstop);

#endif /*I2C_API_*/

