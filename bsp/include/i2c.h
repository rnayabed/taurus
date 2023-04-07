#ifndef __I2C_H
#define __I2C_H
/***************************************************


 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Nov-2019
 Filename		: i2c.h
 Purpose		: I2C Firmware header file
 Description		: I2C functions
 Author(s)		: Karthika P
 Email			: karthikap@cdac.in

 See LICENSE for license details.
 ***************************************************/
/**
 @file i2c.h
 @brief header file for I2C interface
 @detail Includes software functions declarations to initialize,
 configure, write and read over I2C interface
 */
#include "config.h"

typedef unsigned char UC; //1 Byte
typedef unsigned short US; //2 Bytes
typedef unsigned int UI; //4 Bytes
typedef unsigned long UL; //8 Bytes

typedef struct {
	UC I2C_CR;
	UC I2C_SR0;
	UC I2C_SR1;
	UC I2C_IER;
	UC I2C_TxFF;
	UC I2C_RxFF;
	UC I2C_CHL;
	UC I2C_CHH;
	UC I2C_CHHL;
	UC I2C_CHHH;
	UC I2C_TXCLR;
} I2C_REG_TYPE;


#define I2C_0 	0
#define I2C_1	1
#define I2C_ADC	8  // at address 0x1000_1000 


#define I2C_READ 	1
#define I2C_WRITE 	0

#define I2C_EEPROM_ADDR_WR	(0xA2)
#define I2C_EEPROM_ADDR_RD	(0xA3)


//Register address mapping
#define I2CReg(i) (*((volatile I2C_REG_TYPE *)(I2C_BASE_ADDR+ (0x100 * i))))


/*  Function declaration section
 *
 ***************************************************/

void i2c_initialize(UC i2c_number);
UC i2c_start(UC i2c_number, UC read_length, UC Read);
void i2c_configure(UC i2c_number,UL System_Clock, UL I2C_Clock);
UC i2c_data_write(UC i2c_number, UC *write_data, UC write_length);
void i2c_stop(UC i2c_number);
UC i2c_data_read(UC i2c_number, UC *read_data, UC read_length);
int I2C_intr_handler(UC I2C_number);
void I2C_enable_intr(UC i2c_number,UC tx_intr,UC rx_intr);

void i2c_WriteByte_EEPROM(UC i2c_number,UC WBdata, US Word_Address, UC Slave_Address_Wr);
UC i2c_ReadByte_EEPROM(UC i2c_number,UC Slave_Address_Wr,UC Slave_Address_Rd, US Word_Address);
void i2c_WriteMultiByte_EEPROM(UC i2c_num, UC *WBdata, US Word_Address,UC Slave_Address_Wr, UC write_data_length);
void i2c_ReadMultiByte_EEPROM(UC i2c_num, UC *rxd_data, UC Slave_Address_Wr,UC Slave_Address_Rd, US Word_Address, UC read_data_length);

void i2c_WriteByte_SSD1306(UC i2c_number,UC WBdata, US Word_Address, UC Slave_Address_Wr);
UC i2c_ReadByte_SSD1306(UC i2c_number,UC Slave_Address_Wr,UC Slave_Address_Rd, US Word_Address);
void i2c_WriteMultiByte_SSD1306(UC i2c_num, UC *WBdata, US Word_Address,UC Slave_Address_Wr, UC write_data_length);
void i2c_ReadMultiByte_SSD1306(UC i2c_num, UC *rxd_data, UC Slave_Address_Wr,UC Slave_Address_Rd, US Word_Address, UC read_data_length);



#endif /*__I2C_H*/

