/***************************************************


 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Nov-2019
 Filename		: i2c_RTC.c
 Purpose		: I2C RTC interface
 Description		: Sample RTC with I2C interface
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
#include "config.h"
#include "ds3231_rtc.h"

#define SYS_FREQ SYSTEM_FREQUENCY
#define I2C_FREQ 100000

RTC_TIME gRTCtime;
/**
 @fn main
 @brief writes and reads EEPROM with I2C interface
 @details Initializes configures I2C to write, read and compare data in EEPROM
 @param[in] No input parameters.
 @param[Out] No ouput parameter.
 @return Void function.

 */
void main() {
	//UC rxd_data[7];
	printf(" I2C ds3231 RTC\n\r");
	i2c_configure(I2C_1, SYS_FREQ, I2C_FREQ); //System clock =40MHz and I2C clock =100 kHz
	//i2c_initialize(0);

	gRTCtime.sec = 00;
	gRTCtime.min = 30;
	gRTCtime.hour = 07;
	gRTCtime.mday = 9;
	gRTCtime.mon = 6;
	gRTCtime.year = 2022;
	gRTCtime.wday = 5;
	gRTCtime.year_s = 22;

	printf(" set value : %02d-%02d-%02d, %02d:%02d:%02d \n\r", gRTCtime.mday, gRTCtime.mon,
				gRTCtime.year_s, gRTCtime.hour, gRTCtime.min, gRTCtime.sec);
	i2c_set_rtc_DS3231(I2C_1, DS3231_TIME_CAL_ADDR, DS3231_I2C_ADDR_WR);
	printf(" write completed\n\r");
	while (1) {
		i2c_get_rtc_DS3231(I2C_1, DS3231_I2C_ADDR_WR,
		DS3231_I2C_ADDR_RD, DS3231_TIME_CAL_ADDR);

		printf("\r %02x-%02x-%02x, %02x:%02x:%02x, ", gRTCtime.mday, gRTCtime.mon,
				gRTCtime.year_s, gRTCtime.hour, gRTCtime.min, gRTCtime.sec);
		switch (gRTCtime.wday) {
		case 1:
			printf("Sunday   ");
			break;
		case 2:
			printf("Monday   ");
			break;
		case 3:
			printf("Tuesday  ");
			break;
		case 4:
			printf("Wednesday");
			break;
		case 5:
			printf("Thursday ");
			break;
		case 6:
			printf("Friday   ");
			break;
		case 7:
			printf("Saturday ");
			break;

		}
		udelay(2000);
	}

	//printf("success \n\r");
	//while (1)
	//;
}

/**
 @fn i2c_set_rtc_DS3231
 @brief set rtc with i2c interface
 @details write data after writing slave address and word address
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[in] unsigned short (RTC_Address--word RTC_Address)
 @param[in] unsigned short (Slave_Address_Wr--Slave address for write)
 @param[Out] No ouput parameter.
 @return Void function.

 */
void i2c_set_rtc_DS3231(UC i2c_num, UC RTC_Address, UC Slave_Address_Wr) {

	UC i, century;
	century = 0;
	static UC TimeDate[7];
	/*
	 if (gRTCtime.year >= 2000) {	//century calculation
	 century = 0x80;
	 gRTCtime.year_s = gRTCtime.year - 2000;
	 } else {
	 century = 0;
	 gRTCtime.year_s = gRTCtime.year - 1900;
	 }
	 */

	TimeDate[0] = gRTCtime.sec;
	TimeDate[1] = gRTCtime.min;
	TimeDate[2] = gRTCtime.hour;
	TimeDate[3] = gRTCtime.wday;
	TimeDate[4] = gRTCtime.mday;
	TimeDate[5] = gRTCtime.mon;

	TimeDate[6] = gRTCtime.year_s;

	for (i = 0; i <= 6; i++) {
		TimeDate[i] = DecToBcd(TimeDate[i]);
		//if (i == 5)
			//TimeDate[5] += century;
		//printf("\n\r Time data  %x",TimeDate[i]);
	}

	while (1) {
		i2c_start(i2c_num, 0x00, 0);

		if (i2c_data_write(i2c_num, &Slave_Address_Wr, 01)) { //writes slave address -68 and set i2c to write mode

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, &RTC_Address, 01)) { //writes Time calc address 00

			continue; //received NACK
		}

		if (i2c_data_write(i2c_num, TimeDate, 07)) { //writes data

			continue; //received NACK
		}

		i2c_stop(i2c_num);
		break;
	}
}

/**
 @fn i2c_get_rtc_DS3231
 @brief read rtc with I2C interface
 @details read  data after writing slave address and rtc address
 @param[in] unsigned char(i2c_num--which i2c to be used)
 @param[in] unsigned char (Slave_Address_Wr--Slave address for write)
 @param[in] unsigned char (Slave_Address_Rd--Slave address for read)
git pull @param[in] unsigned short (RTC_Address--starting word address)
 @param[in] unsigned long (read_data_length--no:of bytes to be read)
 @param[Out] nil
 @return Void function.

 */

void i2c_get_rtc_DS3231(UC i2c_num, UC Slave_Address_Wr,
		UC Slave_Address_Rd, UC RTC_Address) {

	static UC TimeDate[7];        //second,minute,hour,dow,day,month,year
	UC century = 0;
	UC i, n;
	US year_full;

	while (1) {
		i2c_start(i2c_num, 0x00, 0);

		if (i2c_data_write(i2c_num, &Slave_Address_Wr, 01)) { //writes slave address

			continue; //received NACK
		}
		//printf("Slave address ACK\n\r");
		if (i2c_data_write(i2c_num, &RTC_Address, 01)) { //writes MSB of address

			continue; //received NACK
		}
		//printf("Word address  ACK\n\r");
		i2c_stop(i2c_num);

		i2c_start(i2c_num, 07, 01); //start sequence for reading data
		if (i2c_data_write(i2c_num, &Slave_Address_Rd, 01)) { //write slave address and set rtc to read mode

			continue; //received NACK
		}
		//printf("Slave address read ACK\n\r");

		i2c_data_read(i2c_num, TimeDate, 7);
		/*
		for (i = 0; i <= 6; i++) {
			if ((i == 5)||(i==2)) {
				TimeDate[i] = BcdToDec(TimeDate[i] & 0x1F);
				//century = (rxd_data[i] & 0x80) >> 7;
			} else
				TimeDate[i] = BcdToDec(TimeDate[i]);
			//printf("Time Data %d", TimeDate[i]);
		}
		*/
		/*
		 if (century == 1) {
		 year_full = 2000 + TimeDate[6];
		 } else {
		 year_full = 1900 + TimeDate[6];
		 }
		 */
		gRTCtime.sec = TimeDate[0];
		gRTCtime.min = TimeDate[1];
		gRTCtime.hour = TimeDate[2];
		gRTCtime.wday = TimeDate[3];
		gRTCtime.mday = TimeDate[4];
		gRTCtime.mon = TimeDate[5];
		gRTCtime.year_s = TimeDate[6];
		gRTCtime.year = year_full;



		break;

	}

}

/**
 @fn DecToBcd
 @brief decimal to bcd
 @details converts decimal to bcd
 @param[in] int(val--data to be converted)
 @param[Out] nil
 @return unsigned char(converted bcd number)

 */
UC DecToBcd(UC val) {
	return (UC) (((val / 10) * 16) + (val % 10));
}
/**
 @fn BcdToDec
 @brief bcd to decimal
 @details converts bcd to decimal
 @param[in] Unsigned char(val--data to be converted)
 @param[Out] nil
 @return unsigned char(converted bcd number)

 */

UC BcdToDec(UC val) {
	return (UC) (((val / 16 )* 10) + (val % 16));
}
