/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 06-Nov-2020
 Filename		: i2c_api.c
 Purpose		: I2C high level api for i2c programming
 Description	: I2C high level api for i2c programming
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/

#include "i2c.h"
#include "i2c_api.h"
#include "stdlib.h"

#define I2C_READ	1
#define I2C_WRITE	0

unsigned char rxBuffer[BUFFER_LENGTH];
unsigned char rxBufferIndex = 0;
unsigned char rxBufferLength = 0;

unsigned char txAddress=0, i2c_number=0, transmitting=0;
unsigned char txBuffer[BUFFER_LENGTH];
unsigned char txBufferIndex = 0;
unsigned char txBufferLength = 0;

void i2c_init(unsigned char i2c_num){
	i2c_number=i2c_num;
	i2c_configure(i2c_number, 40000000, 100000);
}


void i2c_beginTransmission(unsigned char i2c_address) {
	i2c_start(i2c_number,0,0);
	txAddress=i2c_address<<1|I2C_WRITE;
	transmitting = 1;
	txBufferIndex = 0;
	txBufferLength = 0;
}

int i2c_write(unsigned char data) {
	if(transmitting)
	{
		if (txBufferLength >= BUFFER_LENGTH)
			{
				printf("Buffer overflow\n");
				return 0;
			}
			txBuffer[txBufferIndex] = data;
			++txBufferIndex;
			txBufferLength = txBufferIndex;
	}
	else
	{
		i2c_data_write(i2c_number, &data, 1);
	}
	return 1;
}

int i2c_multiWrite(char *data,int length) {
	if(transmitting)
	{
		 for (int i = 0; i < length; ++i)
		        {
		            if (!i2c_write(data[i]))
		            {
		                return i;
		            }
		        }
	}
	else
	{
		i2c_data_write(i2c_number, data, length);
	}
	return 1;
}

char i2c_read(void)
{
	char value = -1;
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }
    return value;
}

int i2c_requestFrom(int address, int size, unsigned char sendstop)
{
	 if (size > BUFFER_LENGTH)
	    {
	        size = BUFFER_LENGTH;
	    }
	 	i2c_start(i2c_number, size, 01);
	 	unsigned char address_read= address<<1|I2C_READ;
	 	unsigned char ret = i2c_data_write(i2c_number, &address_read, 1);
	    int read = (i2c_data_read(i2c_number, rxBuffer, size) == 0) ? size : 0;
	    if(sendstop)
	    		i2c_stop(i2c_number);
	    rxBufferIndex = 0;
	   	rxBufferLength = read;
	    return read;
}

unsigned char i2c_endTransmission(unsigned char sendstop) {
	unsigned char ret = i2c_data_write(i2c_number, &txAddress, 1);
	ret = i2c_data_write(i2c_number, txBuffer, txBufferLength);
	if(sendstop)
		i2c_stop(i2c_number);
	txBufferIndex = 0;
	txBufferLength = 0;
	transmitting = 0;
	return ret;
	}
/*unsigned char i2c_endTransmission() {
	unsigned char ret = i2c_data_write(i2c_number, &txAddress, 1);
	i2c_stop(i2c_number);
	return ret;


}*/



