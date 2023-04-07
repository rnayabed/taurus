/***************************************************
 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 9-Aug-2021
 Filename		: bmp180_sensor_demo.c
 Purpose		: I2C BMP180 demo
 Description	: Sample BMP180 with I2C interface
 Author(s)	   	: Thomas Sebastian
 Email			: vega@cdac.in
 
 See LICENSE for license details.
 ***************************************************/

/**
 @file bmp180_sensor_demo.c
 @brief contains routines for I2C BMP180 interface
 @detail Includes software functions declarations to initialize,
 configure, write and read BMP180 over I2C interface
 */

#include "i2c.h"
#include "stdlib.h"
#include "i2c_api.h"
#include "SFE_BMP180.h"
#include <math.h>

#define TRUE 1
#define FALSE 0

int16_t AC1, AC2, AC3, VB1, VB2, MB, MC, MD;
uint16_t AC4, AC5, AC6;
double c5, c6, mc, md, x0, x1, x2, Y0, Y1, y2, p0, p1, p2;
char _error;

char begin()
// Initialize library for subsequent pressure measurements
{
	double c3, c4, b1;

	i2c_init(I2C_0);

	// The BMP180 includes factory calibration data stored on the device.
	// Each device has different numbers, these must be retrieved and
	// used in the calculations when taking pressure measurements.

	// Retrieve calibration data from device:
	//printf("Inint over\n");

	readInt(0xAA, &AC1);
	readInt(0xAC, &AC2);
	readInt(0xAE, &AC3);
	readUInt(0xB0, &AC4);
	readUInt(0xB2, &AC5);
	readUInt(0xB4, &AC6);
	readInt(0xB6, &VB1);
	readInt(0xB8, &VB2);
	readInt(0xBA, &MB);
	readInt(0xBC, &MC);
	readInt(0xBE, &MD);

	//printf("read over\n");
	// All reads completed successfully!

	// If you need to check your math using known numbers,
	// you can uncomment one of these examples.
	// (The correct results are commented in the below functions.)

	// Example from Bosch datasheet
	// AC1 = 408; AC2 = -72; AC3 = -14383; AC4 = 32741; AC5 = 32757; AC6 = 23153;
	// B1 = 6190; B2 = 4; MB = -32768; MC = -8711; MD = 2868;

	// Example from http://wmrx00.sourceforge.net/Arduino/BMP180-Calcs.pdf
	// AC1 = 7911; AC2 = -934; AC3 = -14306; AC4 = 31567; AC5 = 25671; AC6 = 18974;
	// VB1 = 5498; VB2 = 46; MB = -32768; MC = -11075; MD = 2432;

	/*
		printf("\n AC1:%d ",AC1);
		printf("\n AC2:%d ",AC2); 
		printf("\n AC3:%d ",AC3); 
		printf("\n AC4:%d ",AC4); 
		printf("\n AC5: %d ",AC5);
		printf("\n AC6: %d ",AC6);
		printf("\n VB1: %d ",VB1);
		printf("\n VB2: %d ",VB2);
		printf("\n MB: %d ",MB); 
		printf("\n MC: %d ",MC); 
		printf("\n MD: %d ",MD); 
	*/

	// Compute floating-point polynominals:

	c3 = 160.0 * pow(2, -15) * AC3;
	c4 = pow(10, -3) * pow(2, -15) * AC4;
	b1 = pow(160, 2) * pow(2, -30) * VB1;
	c5 = (pow(2, -15) / 160) * AC5;
	c6 = AC6;
	mc = (pow(2, 11) / pow(160, 2)) * MC;
	md = MD / 160.0;
	x0 = AC1;
	x1 = 160.0 * pow(2, -13) * AC2;
	x2 = pow(160, 2) * pow(2, -25) * VB2;
	Y0 = c4 * pow(2, 15);
	Y1 = c4 * c3;
	y2 = c4 * b1;
	p0 = (3791.0 - 8.0) / 1600.0;
	p1 = 1.0 - 7357.0 * pow(2, -20);
	p2 = 3038.0 * 100.0 * pow(2, -36);

	/*	
		printf("\n c3:%f ",(float)c3); 
		printf("\n c4:%f ", (float)c4 );
		printf("\n c5:%f ",  (float)c5);
		printf("\n c6: %f ", (float)c6);
		printf("\n b1: %f ",(float)b1); 
		printf("\n mc: %f ",(float) mc);
		printf("\n md: %f ",(float) md);
		printf("\n x0: %f ", (float)x0);
		printf("\n x1: %f ",(float)x1 );
		printf("\n x2: %f ", (float)x2);
		printf("\n y0: %f ", (float)Y0);
		printf("\n y1: %f ",(float)Y1 );
		printf("\n y2: %f ", (float)y2);
		printf("\n p0:%f",(float) p0 );
		printf("\n p1: %f ",(float)p1 );
		printf("\n p2: %f ", (float)p2);
	*/

	// Success!
	//	printf("Sucess\n");
	return (1);
}

char readInt(char address, int16_t *value)
// Read a signed integer (two bytes) from device
// address: register to start reading (plus subsequent register)
// value: external variable to store data (function modifies value)
{

	unsigned char data[2];

	data[0] = address;
	if (readBytes(data, 2))
	{
		*value = (int16_t)((data[0] << 8) | data[1]);
		//if (*value & 0x8000) *value |= 0xFFFF0000; // sign extend if negative
		return (1);
	}
	value = 0;
	return (0);
}

char readUInt(char address, uint16_t *value)
// Read an unsigned integer (two bytes) from device
// address: register to start reading (plus subsequent register)
// value: external variable to store data (function modifies value)
{

	unsigned char data[2];

	data[0] = address;
	if (readBytes(data, 2))
	{
		*value = (((uint16_t)data[0] << 8) | (uint16_t)data[1]);
		return (1);
	}
	value = 0;
	return (0);
}

char readBytes(unsigned char *values, char length)
// Read an array of bytes from device
// values: external array to hold data. Put starting register in values[0].
// length: number of bytes to read
{
	char x;

	i2c_beginTransmission(BMP180_ADDR);
	//printf(" Base ADDR : %02x Reg : %02x \n",BMP180_ADDR, values[0]);
	i2c_write(values[0]);
	_error = i2c_endTransmission(TRUE);

	i2c_requestFrom(BMP180_ADDR, length, TRUE);

	for (x = 0; x < length; x++)
	{
		values[x] = i2c_read();
	}
	return (1);
}

char writeBytes(unsigned char *values, char length)
// Write an array of bytes to device
// values: external array of data to write. Put starting register in values[0].
// length: number of bytes to write
{
	char x;

	i2c_beginTransmission(BMP180_ADDR);
	for (x = 0; x < length; x++)
	{
		i2c_write(values[x]);
	}

	_error = i2c_endTransmission(TRUE);

	return (1);
}

char startTemperature(void)
// Begin a temperature reading.
// Will return delay in ms to wait, or 0 if I2C error
{
	unsigned char data[2], result;

	data[0] = BMP180_REG_CONTROL;
	data[1] = BMP180_COMMAND_TEMPERATURE;
	result = writeBytes(data, 2);
	if (result)
	{ // good write?
		usleep(5000);
		return (1); // return the delay in ms (rounded up) to wait before retrieving data
	}
	else
		return (0); // or return 0 if there was a problem communicating with the BMP
}

char getTemperature(double *T)
// Retrieve a previously-started temperature reading.
// Requires begin() to be called once prior to retrieve calibration parameters.
// Requires startTemperature() to have been called prior and sufficient time elapsed.
// T: external variable to hold result.
// Returns 1 if successful, 0 if I2C error.
{
	unsigned char data[2];
	char result;
	double tu, a;

	data[0] = BMP180_REG_RESULT;

	result = readBytes(data, 2);
	//	printf(" temp data : %x %x ", data[0], data[1]);
	if (result) // good read, calculate temperature
	{
		tu = (data[0] * 256.0) + data[1];

		//example from Bosch datasheet
		//tu = 27898;

		//example from http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf
		//tu = 0x69EC;
		/*
		printf("\n c5:%f ",  (float)c5);
		printf("\n c6: %f ", (float)c6);
		printf("\n mc: %f ",(float) mc);
		printf("\n md: %f ",(float) md);
*/
		a = c5 * (tu - c6);
		*T = a + (mc / (a + md));

		/*		printf("\n tu: %f ",(float) tu);
		printf("\n a: %f ",(float) a);
		printf("\n  T:%f ",*T);
*/
	}
	return (result);
}

char startPressure(char oversampling)
// Begin a pressure reading.
// Oversampling: 0 to 3, higher numbers are slower, higher-res outputs.
// Will return delay in ms to wait, or 0 if I2C error.
{
	unsigned char data[2], result;

	data[0] = BMP180_REG_CONTROL;

	switch (oversampling)
	{
	case 0:
		data[1] = BMP180_COMMAND_PRESSURE0;
		usleep(5000);
		break;
	case 1:
		data[1] = BMP180_COMMAND_PRESSURE1;
		usleep(8000);
		break;
	case 2:
		data[1] = BMP180_COMMAND_PRESSURE2;
		usleep(14000);
		break;
	case 3:
		data[1] = BMP180_COMMAND_PRESSURE3;
		usleep(26000);
		break;
	default:
		data[1] = BMP180_COMMAND_PRESSURE0;
		usleep(5000);
		break;
	}
	result = writeBytes(data, 2);
	if (result)
	{ // good write?
		usleep(5000);
		return (1); // return the delay in ms (rounded up) to wait before retrieving data
	}
	else
		return (0); // or return 0 if there was a problem communicating with the BMP
}

char getPressure(double *P, double T)
// Retrieve a previously started pressure reading, calculate abolute pressure in mbars.
// Requires begin() to be called once prior to retrieve calibration parameters.
// Requires startPressure() to have been called prior and sufficient time elapsed.
// Requires recent temperature reading to accurately calculate pressure.

// P: external variable to hold pressure.
// T: previously-calculated temperature.
// Returns 1 for success, 0 for I2C error.

// Note that calculated pressure value is absolute mbars, to compensate for altitude call sealevel().
{
	unsigned char data[3];
	char result;
	double pu, s, x, y, z;

	data[0] = BMP180_REG_RESULT;

	result = readBytes(data, 3);
	if (result) // good read, calculate pressure
	{
		pu = (data[0] * 256.0) + data[1] + (data[2] / 256.0);

		//example from Bosch datasheet
		//pu = 23843;

		//example from http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf, pu = 0x982FC0;
		//pu = (0x98 * 256.0) + 0x2F + (0xC0/256.0);

		/*	printf("\n T:%f ",T);
		printf("\n x0: %f ", (float)x0);
		printf("\n x1: %f ",(float)x1 );
		printf("\n x2: %f ", (float)x2);
		printf("\n y0: %f ", (float)Y0);
		printf("\n y1: %f ",(float)Y1 );
		printf("\n y2: %f ", (float)y2);
		printf("\n p0:%f",(float) p0 );
		printf("\n p1: %f ",(float)p1 );
		printf("\n p2: %f ", (float)p2);
		
	*/

		s = T - 25.0;
		x = (x2 * pow(s, 2)) + (x1 * s) + x0;
		y = (y2 * pow(s, 2)) + (Y1 * s) + Y0;
		z = (pu - x) / y;
		*P = (p2 * pow(z, 2)) + (p1 * z) + p0;

		/*	printf("\n pu: %f ", (float)pu);
		printf("\n pres T:%f ",T);
	*/

		/*  printf("\n pu:%f ",pu);
		printf("\n T:%f ",T);
		printf("\n s:%f ",s);
		printf("\n x:%f ",x);
		printf("\n y:%f ",y);
		printf("\n z:%f ",z);
		printf("\n p:%f ",*P);
	*/
	}
	return (result);
}

double sealevel(double P, double A)
// Given a pressure P (mb) taken at a specific altitude (meters),
// return the equivalent pressure (mb) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
	return (P / pow(1 - (A / 44330.0), 5.255));
}

double altitude(double P, double P0)
// Given a pressure measurement P (mb) and the pressure at a baseline P0 (mb),
// return altitude (meters) above baseline.
{
	return (44330.0 * (1 - pow(P / P0, 1 / 5.255)));
}

char getError(void)
// If any library command fails, you can retrieve an extended
// error code using this command. Errors are from the wire library:
// 0 = Success
// 1 = Data too long to fit in transmit buffer
// 2 = Received NACK on transmit of address
// 3 = Received NACK on transmit of data
// 4 = Other error
{
	return (_error);
}
