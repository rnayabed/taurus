/***************************************************************************
* Module name                           :  main.c
* Name of Author                        :  Karthika P, C-DAC
* Email ID  (Report any bugs)           :  karthikap@cdac.in
* Module Description                    :  W25Q32 SPI Flash Library

  Copyright (C) 2020  CDAC(T). All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/

#include "stdlib.h"
#include "spi.h"
#include "w25q32.h"

/**
 @file w25q32.c
 @brief W25Q32 SPI Flash Library
 @detail 
 */

unsigned char currentSpi = 0;

/** 
 * @fn void W25Q32Begin(unsigned char spiNo)
 * @brief To initialize the W25Q32 flash.
 * @details 
 * @param[in] unsigned char spiNo
 * @param[Out] No output parameter 
*/
void w25q32Begin(unsigned char spiNo)
{
	static char devID[3], cmdBuf[2];
	SPIcntrlRegType w25q32SpiCtrl;
	unsigned char status;

	// Custom SPI control register setup, refer SPI datasheet for more details.
	w25q32SpiCtrl.Bits.PeriphChipSelect = 0;
	w25q32SpiCtrl.Bits.PeriphType = 0;
	w25q32SpiCtrl.Bits.DataOrder = MSB;
	w25q32SpiCtrl.Bits.Mode = 3;					  // SPI mode 3
	w25q32SpiCtrl.Bits.SPIrxTxIntr = RX_TX_INTR_DIS; // no interrupts
	w25q32SpiCtrl.Bits.CSAAT = LOW;
	w25q32SpiCtrl.Bits.Dbits = 0; // 8-bit SPI operation.

	currentSpi = spiNo;
	SPI_init(currentSpi);
	SPI_config(currentSpi, w25q32SpiCtrl); //modifing spi control register.
	SPI_set_baud(currentSpi, SPI_BAUD_CFD_4);	// setting spi baud rate.
	w25q32Read(devID, 3, READ_DEV_ID);
	printf("[INFO] flash %02x:%02x:%02x initialized at spi%d.\n", devID[0], devID[1], devID[2],currentSpi);


}

/** 
 * @fn void w25q32ReadEeprom(char *readBuf, int length, unsigned int eepromAddress)
 * @brief To read sst25vf040b eeprom data.
 * @details 
 * @param[in]  int length, unsigned int eepromAddress
 * @param[Out] char *readBuf,
*/
void w25q32ReadFlash(char *readBuf, int length, unsigned int flashAddress)
{
	char cmdBuf[4];

	cmdBuf[0] = READ_FLASH;				  //read command.
	cmdBuf[1] = (flashAddress >> 16) & 0xFF; // address bits 15-23
	cmdBuf[2] = (flashAddress >> 8) & 0xFF;  // address bits  8-15
	cmdBuf[3] = (flashAddress)&0xFF;		  // address bits  0-7

	printf("[INFO] Read cmd 0x%x at 0x%02x%02x%02x of length %d bytes.\n", cmdBuf[0], cmdBuf[1], cmdBuf[2], cmdBuf[3], length);

	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.

	// flash read command.
	SPI_transmit(currentSpi, cmdBuf[0]);
	SPI_transmit(currentSpi, cmdBuf[1]);
	SPI_transmit(currentSpi, cmdBuf[2]);
	SPI_transmit(currentSpi, cmdBuf[3]);
	// reading data.
	while (length)
	{
		SPI_transmit(currentSpi, DUMMY_DATA);
		*readBuf = SPI_receive(currentSpi); //data
		readBuf++;
		length--; // since 2 bytes are read in one time.
	}
	SPI_set_CSAAT_pin(currentSpi, LOW); // Setting CSAAT bit low.
}

/** 
 * @fn void w25q32WriteFlash(char *writeBuf, int length, unsigned int flashAddress)
 * @brief To write data to w25q32 flash.
 * @details 
 * @warning Min Write length should not be less than 2 and length should be an even number.
 * @param[in] char *writeBuf, int length, unsigned int eepromAddress
 * @param[Out] No output parameter 
*/
void w25q32WriteFlash(char *writeBuf, int length, unsigned int flashAddress)
{

	char cmdBuf[4];
	if(length>=256){
	printf("WARN: No: of bytes to be written is greater than 256.\n");
	printf("WARN: May overwrite data \n");
	}

	cmdBuf[0] = WRITE_ENABLE;
	w25q32Write(cmdBuf, 1);

	cmdBuf[0] = PAGE_PROGRAM;				  // write command.
	cmdBuf[1] = (flashAddress >> 16) & 0xFF; // address bits 15-23
	cmdBuf[2] = (flashAddress >> 8) & 0xFF;  // address bits  8-15
	cmdBuf[3] = (flashAddress)&0xFF;		  // address bits  0-7

	printf("[INFO] Write cmd 0x%x at 0x%02x%02x%02x of length %d bytes.\n", cmdBuf[0], cmdBuf[1], cmdBuf[2], cmdBuf[3], length);
	w25q32Write(cmdBuf, 4);

	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
		SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.
	for (int i=0;i<4;i++)
		{
			SPI_transmit(currentSpi,cmdBuf[i]);
		}

	while (length)
		{
			SPI_transmit(currentSpi, *writeBuf);
			writeBuf++;
			length--;
		}

	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.
	w25q32BusyWait();


}

/** 
 * @fn void w25q32Read(char *readBuf, int length, char opcode)
 * @brief To read registers of w25q32 flash.
 * @details 
 * @param[in] , int length, char opcode
 * @param[Out] char *readBuf
*/
void w25q32Read(char *readBuf, int length, char opcode)
{
	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.
	SPI_transmit(currentSpi, opcode);
	while (length)
	{

		SPI_transmit(currentSpi, DUMMY_DATA);//not in datasheet
		*readBuf = SPI_receive(currentSpi); //data
		readBuf++;
		length--; // since 2 bytes are read in one time.
	}
	SPI_set_CSAAT_pin(currentSpi, LOW); // Setting CSAAT bit low.
}

/** 
 * @fn void w25q32Write(char *writeBuf, int length)
 * @brief To write command to w25q32 flash.
 * @details 
 * @param[in] char *writeBuf, int length
 * @param[Out] No output parameter 
*/
void w25q32Write(char *writeBuf, int length)
{
	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.
	while (length)
	{
		SPI_transmit(currentSpi, *writeBuf);
		writeBuf++;
		length--; // since 2 bytes are read in one time.
	}
	SPI_set_CSAAT_pin(currentSpi, LOW); // Setting CSAAT bit low.
}


/** 
 * @fn void w25q32ChipErase(void)
 * @brief For compleate w25q32 flash erase.
 * @details 
 * @param[in] No output parameter 
 * @param[Out] No output parameter 
*/
void w25q32ChipErase(void)
{
	static char opcode;

	SPI_wait_if_busy(currentSpi); // Check if SPI controller is busy.
	printf("[WARN] executing full chip erase at spi%d.\n", currentSpi);
	opcode = WRITE_ENABLE;
	w25q32Write(&opcode, 1);
	opcode = CHIP_ERASE;
	w25q32Write(&opcode, 1);
	SPI_set_CSAAT_pin(currentSpi, LOW); // Setting CSAAT bit low.
	w25q32BusyWait();
	printf("[INFO] chip erase done at spi%d.\n", currentSpi);
}

/** 
 * @fn void w25q32BusyWait(void)
 * @brief Wait till w25q32 flash is busy.
 * @details 
 * @param[in] No output parameter 
 * @param[Out] No output parameter 
*/
void w25q32BusyWait(void)
{
	unsigned char status;

	do
	{
		w25q32Read(&status, 1, READ_STATUS);
	} while (status & EEPROM_BUSY);
}
