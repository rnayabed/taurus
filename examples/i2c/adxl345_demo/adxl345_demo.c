/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 06-Nov-2020
 Filename		: main.c
 Purpose		: I2C adxl345 demo
 Description	: Sample adxl345 with I2C interface
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file main.c
 @brief contains routines for I2C ADXL345 interface
 @detail Includes software functions declarations to initialize,
 configure, write and read ADXL345 over I2C interface
 */

#include "i2c.h"
#include "stdlib.h"
#include "i2c_api.h"

#define TRUE 1
#define FALSE 0
// Wire library - used for I2C communication
int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs
signed short sX_out, sY_out, sZ_out;  // Outputs

void main() {
	printf("ADXL345\n");
	i2c_init(I2C_1); //System clock =40MHz and I2C clock =100 kHz
	i2c_beginTransmission(ADXL345); // Start communicating with the device
	i2c_write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
	// Enable measurement
	i2c_write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
	i2c_endTransmission(TRUE);
	udelay(100);

	i2c_beginTransmission(ADXL345);
	i2c_write(0x31);  // Range set to 16g/ Full resolution
	i2c_write(0xb);
	i2c_endTransmission(TRUE);
	udelay(100);

	//X-axis
	i2c_beginTransmission(ADXL345);
	i2c_write(0x1E);  // X-axis offset register
	i2c_write(97);
	i2c_endTransmission(TRUE);
	udelay(100);

	//Y-axis
	i2c_beginTransmission(ADXL345);
	i2c_write(0x1F);  // Y-axis offset register
	i2c_write(255);
	i2c_endTransmission(TRUE);
	udelay(100);

	//Z-axis
	i2c_beginTransmission(ADXL345);
	i2c_write(0x20);  // Z-axis offset register
	i2c_write(-9);
	i2c_endTransmission(TRUE);
	udelay(100);

	while (1) {
		// === Read acceleromter data === //
		i2c_beginTransmission(ADXL345);
		i2c_write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
		i2c_endTransmission(TRUE);
		i2c_requestFrom(ADXL345, 6, TRUE); // Read 6 registers total, each axis value is stored in 2 registers
		sX_out = ((i2c_read() << 8) | i2c_read()); // X-axis value
		sY_out = ((i2c_read() << 8) | i2c_read()); // Y-axis value
		sZ_out = ((i2c_read() << 8) | i2c_read()); // Z-axis value

		//printf("\rXa= %d,  Ya= %d, Za= %d",sX_out,sY_out,sZ_out);
		
		udelay(9000);
		
		X_out = ((float) sX_out) / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
		Y_out = ((float) sY_out) / 256;
		Z_out = ((float) sZ_out) / 256;

		printf("\rXa= %5.2f,  Ya= %5.2f, Za= %5.2f", X_out, Y_out, Z_out);
		

	}
}
