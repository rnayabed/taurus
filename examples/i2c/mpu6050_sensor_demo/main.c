/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 23-mar-2021
 Filename		: main.c
 Purpose		: I2C MPU6050 demo
 Description	: Sample MPU6050 with I2C interface
 Author(s)	   	: thomas sebastian
 Email			: vega@cdac.in
 
 See LICENSE for license details.
 ***************************************************/

/**
 @file main.c
 @brief contains routines for I2C MPU6050 interface
 @detail Includes software functions declarations to initialize,
 configure, write and read MPU6050 over I2C interface
 */

#include "i2c.h"
#include "stdlib.h"
#include "i2c_api.h"
#include "MPU6050_res_define.h"

#define TRUE 1
#define FALSE 0

int mpu6050 = 0x68; // The MPU6050 sensor I2C address
float Xa,Ya,Za,t,Xg,Yg,Zg; // final Outputs
signed short Ax,Ay,Az,T,Gx,Gy,Gz; // raw Outputs

void main() {

 	printf("mpu6050\n"); 
 	
	i2c_init(I2C_1); //System clock =75MHz and I2C clock =100 kHz
	
	// Start communicating with the device
	
	i2c_beginTransmission(mpu6050); // Start with device write address 
	i2c_write(SMPLRT_DIV); 	 //Write to sample rate register
	i2c_write(0x07); 		 //1KHz sample rate
	i2c_endTransmission(TRUE);
	udelay(100);

	i2c_beginTransmission(mpu6050);
	i2c_write(PWR_MGMT_1);  	//Write to power management register
	i2c_write(0x01);		//X axis gyroscope reference frequency
	i2c_endTransmission(TRUE);
	udelay(100);
	
	
	i2c_beginTransmission(mpu6050);
	i2c_write(CONFIG);  		// Write to Configuration register 
	i2c_write(0x00);		//Fs = 8KHz
	i2c_endTransmission(TRUE);
	udelay(100);
	
	
	i2c_beginTransmission(mpu6050);
	i2c_write(GYRO_CONFIG);  	//Write to Gyro configuration register 
	i2c_write(0x18);		//Full scale range +/- 2000 degree/C
	i2c_endTransmission(TRUE);
	udelay(100);
	
	
	/* // interrupt enable
	i2c_beginTransmission(mpu6050);
	i2c_write(INT_ENABLE);  	// Write to interrupt enable register
	i2c_write(0x01);
	i2c_endTransmission(TRUE);
	udelay(100);
	*/

	while (1) {
			i2c_beginTransmission(mpu6050);
			i2c_write(ACCEL_XOUT_H); // Start with register ACCEL_XOUT_H
			i2c_endTransmission(TRUE);
			i2c_requestFrom(mpu6050, 14, TRUE); 
			
			
			Ax = ((i2c_read() << 8) | i2c_read()); //Accelerometer x-axis raw data 
			Ay = ((i2c_read() << 8) | i2c_read()); // Accelerometer y-axis raw data 
			Az = ((i2c_read() << 8) | i2c_read()); // Accelerometer z-axis raw data 
			T  = ((i2c_read() << 8) | i2c_read()); // //temperature raw data
			Gx = ((i2c_read() << 8) | i2c_read()); // Gyro x axis raw data 
			Gy = ((i2c_read() << 8) | i2c_read()); // Gyro y axis raw data 
			Gz = ((i2c_read() << 8) | i2c_read()); // Gyro z axis raw data 
			i2c_endTransmission(TRUE);
			
			
		udelay(30000);
			
		
			
			Xa = (float)Ax/16384.0; //Accelerometer x-axis data in g unit
			Ya = (float)Ay/16384.0; // Accelerometer y-axis data in g unit
			Za = (float)Az/16384.0; // Accelerometer z-axis data in g unit
			Xg = (float)Gx/131.0;   // Gyro x axis data in degree/seconds unit
			Yg = (float)Gy/131.0;   // Gyro y axis data in degree/seconds unit
			Zg = (float)Gz/131.0;	 // Gyro z axis data in degree/seconds unit
			t = ((float)T/340.00)+36.53;	//temperature in degree/Celcius , Convert temperature in °/c using formula 
			
			
			printf("\rXa= %5.2f,  Ya= %5.2f, Za= %5.2f,Xg= %5.2f,  Yg= %5.2f, Zg= %5.2f,t= %5.2f", Xa, Ya, Za,Xg,Yg,Zg,t); // final out
		

}
}
