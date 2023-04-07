#ifndef _EEPROM_SPI_H
#define _EEPROM_SPI_H
/**
 @file m25p80_eeprom.h
 @brief header file for m25p80_eeprom 
 @detail 
 */
/***************************************************
* Module name: M25P80_spi.h
*
* Copyright 2020 Company CDAC(T).
* All Rights Reserved.
*
*  The information contained herein is confidential
* property of Company. The user, copying, transfer or
* disclosure of such information is prohibited except
* by express written agreement with Company.
*
*
* Module Description:
* SPI registers and function declarations
*
***************************************************/

/*  Include section
*
***************************************************/

/*  Define section
* 
*
***************************************************/


/*************Micron M25P80 Serial Flash************* 
*************Embedded Memory Command Set************/
#define WR_DISABLE_LATCH_SPI_CMD			0x04
#define WR_EN_LATCH_SPI_CMD     			0x06
#define BYTE_PAGE_PGM_SPI_CMD  				0x02
#define RD_DATA_BYTES_SPI_CMD  				0x03
#define RD_STATUS_REG_SPI_CMD				0x05
#define RDID_SPI_CMD					0x83

/***************************************************/
 

/*  Function declaration section
* 
*
***************************************************/

UC memcmp(UC *bWrData, UC *bRdData, UI wDataLength);
void ProgramBytePageSPI(UC spi_number,UL wAddress, UC *pbData,UL wDatalength);
UC ReadStatusRegSPI(UC spi_number);
void ReadDataBytesSPI(UC spi_number,UL wAddress, UC *pbData, UL wDatalength);
void TestSPI_Few_Locations(UC spi_number);
#endif	/* _EEPROM_SPI_H */	



