/***************************************************/

/*  Include section
*
*
***************************************************/
#ifndef I2C_API_H_
#define I2C_API_H_

#include "stdlib.h"
#include "i2c.h"

void i2c_init(unsigned char i2c_num);
void i2c_beginTransmission(unsigned char i2c_address);
unsigned char i2c_endTransmission();


#endif /*I2C_API_*/

