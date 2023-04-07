/***************************************************


 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Nov-2019
 Filename		: i2c_EEEPROM.c
 Purpose		: I2C EEPROM interface
 Description		: Sample EEPROM with I2C interface
 Author(s)		: Karthika P
 Email			: karthikap@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file i2c_EEPROM.c
 @brief contains routines for I2C EEPROM interface
 @detail Includes software functions declarations to initialize,
 configure, write and read EEPROM over I2C interface
 */

#include "i2c.h"
#include "stdlib.h"


#define I2C_FREQ 400000
/**
 @fn main
 @brief writes and reads EEPROM with I2C interface
 @details Initializes configures I2C to write, read and compare data in EEPROM
 @param[in] No input parameters.
 @param[Out] No ouput parameter.
 @return Void function.

 */
void main() {

	printf("I2C EEPROM-24aa64\n\r");
	i2c_configure(I2C_0, SYSTEM_FREQUENCY, I2C_FREQ); //System clock =25MHz and I2C clock =100 kHz
	//i2c_initialize(0);

	printf("I2C EEPROM Write started 1 byte \n\r");

	i2c_WriteByte_EEPROM(I2C_0, 0x02, 0x0000, I2C_EEPROM_ADDR_WR);//control code 0A,chip select 0
	printf("I2C EEPROM Read started 1 byte \n\r");
	UC Byte_data = i2c_ReadByte_EEPROM(I2C_0, I2C_EEPROM_ADDR_WR, I2C_EEPROM_ADDR_RD, 0x0000);//control code 0A,chip select 0

	if (Byte_data == 0x02) {
		printf("Rxd character is 0x02 \n\r");
		printf("Byte write and read successfull \n\r");

	} else {
		printf("Byte write and read failure \n\r");

	}
	UC data_arr[16];
	UC rxd_data[16];
	UL rand_value = 99;
	printf("Array initialisation \n\r");
	for (UL i = 0; i < 16; i++) {	//array initialisation

		rand_value = rand_value * 5;
		if (rand_value == 0)
			rand_value = 99;
		data_arr[i] = (UC) rand_value;
		//printf("data to be wriiten %x", data_arr[i]);
		//printf("\n\r");
	}
	printf("I2C EEPROM Write started \n\r");
	i2c_WriteMultiByte_EEPROM(I2C_0, data_arr, 0x0000, I2C_EEPROM_ADDR_WR, 16);
	printf("I2C EEPROM Read started\n\r");
	i2c_ReadMultiByte_EEPROM(I2C_0, rxd_data, I2C_EEPROM_ADDR_WR, I2C_EEPROM_ADDR_RD, 0x0000, 16);
	for (int i = 0; i < 16; i++)	//comparison
		if (rxd_data[i] != data_arr[i]) {
			printf("read unsuccessfull \n\r");
			printf("rxd data %x", rxd_data[i]);
			printf("; ref data %x", data_arr[i]);
			printf("\n\r");
			while (1)
				;

		}

	printf("success \n\r");
	while (1)
		;
}

/**
 @fn i2c_WriteByte_EEPROM
 @brief writes a byte of data to EEPROM with I2C interface
 @details write 1 byte data after writing slave address and word address
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[in] unsigned char (WBdata--data to be written)
 @param[in] unsigned short (Word_Address--word address)
 @param[in] unsigned short (Slave_Address_Wr--Slave address for write)
 @param[Out] No ouput parameter.
 @return Void function.

 */
void i2c_WriteByte_EEPROM(UC i2c_num, UC WBdata, US Word_Address,
		UC Slave_Address_Wr) {
	UC Word_addr_MSB = (UC) (Word_Address >> 8);
	UC Word_addr_LSB = (UC) (Word_Address & 0xFF);

	while (1) {
		i2c_start(i2c_num, 0x00, I2C_WRITE);

		if (i2c_data_write(i2c_num, &Slave_Address_Wr, 01)) { //writes slave address and set EEPROM to write mode

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, &Word_addr_MSB, 01)) { //writes MSB of address

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, &Word_addr_LSB, 01)) { //writes LSB of address

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, &WBdata, 01)) { //writes data

			continue; //received NACK
		}

		i2c_stop(i2c_num);
		break;

	}
}

/**
 @fn i2c_ReadByte_EEPROM
 @brief reads a byte of data to EEPROM with I2C interface
 @details reads 1 byte data after writing slave address and word address
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[in] unsigned short (Slave_Address_Wr--Slave address for write)
 @param[in] unsigned short (Slave_Address_Rd--Slave address for read)
 @param[in] unsigned short (Word_Address--word address)
 @param[Out] No ouput parameter.
 @return unsigned char data from EEPROM

 */
UC i2c_ReadByte_EEPROM(UC i2c_num, UC Slave_Address_Wr, UC Slave_Address_Rd,
		US Word_Address) {
	UC Word_addr_MSB = (UC) (Word_Address >> 8);
	UC Word_addr_LSB = (UC) (Word_Address & 0xFF);
	UC rxd_data;

	while (1) {
		i2c_start(i2c_num, 0x00, I2C_WRITE);

		if (i2c_data_write(i2c_num, &Slave_Address_Wr, 01)) { //writes slave address

			continue; //received NACK
		}
		//printf("Slave address ACK\n\r");
		if (i2c_data_write(i2c_num, &Word_addr_MSB, 01)) { //writes MSB of address

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, &Word_addr_LSB, 01)) { //writes LSB of address

			continue; //received NACK
		}
		//printf("Word address  ACK\n\r");
		i2c_stop(i2c_num);

		i2c_start(i2c_num, 0x01, I2C_READ); //start sequence for reading data
		if (i2c_data_write(i2c_num, &Slave_Address_Rd, 01)) { //write slave address and set EEPROM to read mode

			continue; //received NACK
		}
		//printf("Slave address read ACK\n\r");
		i2c_data_read(i2c_num, &rxd_data, 01);
		printf("data is %x", rxd_data);
		printf("\n\r");
		return rxd_data;

	}

}

/**
 @fn i2c_WriteMultiByte_EEPROM
 @brief writes given number of byte of data to EEPROM with I2C interface
 @details write bytes data after writing slave address and word address
 @param[in] unsigned char(i2c_num--which i2c to be used)
 @param[in] unsigned char(*WBdata--address of the data array(to be written to EEPROM))
 @param[in] unsigned short (Word_Address--starting word address)
 @param[in] unsigned char (Slave_Address_Wr--Slave address for write)
 @param[in] unsigned long (write_data_length--no:of bytes to be written)
 @param[Out] No ouput parameter.
 @return Void function.

 */
void i2c_WriteMultiByte_EEPROM(UC i2c_num, UC *WBdata, US Word_Address,
		UC Slave_Address_Wr, UC write_data_length) {
	UC Word_addr_MSB = (UC) (Word_Address >> 8);
	UC Word_addr_LSB = (UC) (Word_Address & 0xFF);

	while (1) {
		i2c_start(i2c_num, 0x00, 0);

		if (i2c_data_write(i2c_num, &Slave_Address_Wr, 01)) { //writes slave address and set EEPROM to write mode

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, &Word_addr_MSB, 01)) { //writes MSB of address

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, &Word_addr_LSB, 01)) { //writes LSB of address

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, WBdata, write_data_length)) { //writes data

			continue; //received NACK
		}

		i2c_stop(i2c_num);
		break;

	}
}

/**
 @fn i2c_ReadMultiByte_EEPROM
 @brief read given number of byte of data to EEPROM with I2C interface
 @details read bytes data after writing slave address and word address
 @param[in] unsigned char(i2c_num--which i2c to be used)
 @param[in] unsigned char (Slave_Address_Wr--Slave address for write)
 @param[in] unsigned char (Slave_Address_Rd--Slave address for read)
 @param[in] unsigned short (Word_Address--starting word address)
 @param[in] unsigned long (read_data_length--no:of bytes to be read)
 @param[Out] unsigned char (*rxd_data)
 @return Void function.

 */

void i2c_ReadMultiByte_EEPROM(UC i2c_num, UC *rxd_data, UC Slave_Address_Wr,
		UC Slave_Address_Rd, US Word_Address, UC read_data_length) {
	UC Word_addr_MSB = (UC) (Word_Address >> 8);
	UC Word_addr_LSB = (UC) (Word_Address & 0xFF);

	while (1) {
		i2c_start(i2c_num, 0x00, 0);

		if (i2c_data_write(i2c_num, &Slave_Address_Wr, 01)) { //writes slave address

			continue; //received NACK
		}
		printf("Slave address ACK\n\r");
		if (i2c_data_write(i2c_num, &Word_addr_MSB, 01)) { //writes MSB of address

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, &Word_addr_LSB, 01)) { //writes LSB of address

			continue; //received NACK
		}
		printf("Word address  ACK\n\r");
		i2c_stop(i2c_num);

		i2c_start(i2c_num, read_data_length, 01); //start sequence for reading data
		if (i2c_data_write(i2c_num, &Slave_Address_Rd, 01)) { //write slave address and set EEPROM to read mode

			continue; //received NACK
		}
		printf("Slave address read ACK\n\r");
		i2c_data_read(i2c_num, rxd_data, read_data_length);

		//printf("data is %x", rxd_data);
		//printf("\n\r");
		break;

	}

}


