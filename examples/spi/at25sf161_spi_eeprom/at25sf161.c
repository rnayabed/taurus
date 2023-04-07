/***************************************************************************
* Module name                           :  at25sf161.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  AT25SF161 SPI Flash Library

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
#include "at25sf161.h"

/**
 @file at25sf161.c
 @brief AT25SF161 SPI Flash Library 
 @detail 
 */

unsigned char currentSpi = 0;

/** 
 * @fn void at25sf161Begin(unsigned char spiNo) 
 * @brief To initialize the at25sf161 eeprom.
 * @details 
 * @param[in] unsigned char spiNo
 * @param[Out] No output parameter 
*/
void at25sf161Begin(unsigned char spiNo)
{
	static char devID[3];
	SPIcntrlRegType at25sf161SpiCtrl;

	// Custom SPI control register setup, refer SPI datasheet for more details.
	at25sf161SpiCtrl.Bits.PeriphChipSelect = 0;
	at25sf161SpiCtrl.Bits.PeriphType = 0;
	at25sf161SpiCtrl.Bits.DataOrder = MSB;
	at25sf161SpiCtrl.Bits.Mode = 3;						// SPI mode 3
	at25sf161SpiCtrl.Bits.SPIrxTxIntr = RX_TX_INTR_DIS; // no interrupts
	at25sf161SpiCtrl.Bits.CSAAT = LOW;
	at25sf161SpiCtrl.Bits.Dbits = 0; // 8-bit SPI operation.

	currentSpi = spiNo;
	SPI_init(currentSpi);
	SPI_config(currentSpi, at25sf161SpiCtrl); // modifing spi control register.
	SPI_set_baud(currentSpi, SPI_BAUD_CFD_4); // setting spi baud rate.
	at25sf161Read(devID, 3, READ_DEV_ID);
	printf("[INFO] eeprom %02x:%02x:%02x initialized at spi%d.\n", devID[0], devID[1], devID[2], currentSpi);
}

/** 
 * @fn void at25sf161ReadEeprom(char *readBuf, int length, unsigned int eepromAddress)
 * @brief To read at25sf161 eeprom data.
 * @details 
 * @param[in] char *readBuf, int length, unsigned int eepromAddress
 * @param[Out] No output parameter 
*/
void at25sf161ReadEeprom(char *readBuf, int length, unsigned int eepromAddress)
{
	char cmdBuf[4];

	cmdBuf[0] = READ_EEPROM;				  //read command.
	cmdBuf[1] = (eepromAddress >> 16) & 0xFF; // address bits 15-23
	cmdBuf[2] = (eepromAddress >> 8) & 0xFF;  // address bits  8-15
	cmdBuf[3] = (eepromAddress)&0xFF;		  // address bits  0-7

	printf("[INFO] Read cmd 0x%x at 0x%02x%02x%02x of length %d bytes.\n", cmdBuf[0], cmdBuf[1], cmdBuf[2], cmdBuf[3], length);

	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.

	// eeprom read command.
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
 * @fn void at25sf161WriteEeprom(char *writeBuf, int length, unsigned int eepromAddress)
 * @brief To write data to at25sf161 eeprom.
 * @details 
 * @warning Max Write length should not be greater than page size (256).
 * @param[in] char *writeBuf, int length, unsigned int eepromAddress
 * @param[Out] No output parameter 
*/
void at25sf161WriteEeprom(char *writeBuf, int length, unsigned int eepromAddress)
{
	char cmdBuf[4];

	if (length > PAGE_SIZE)
	{
		printf("[EROR] Write length is large than page size(%d).\n", PAGE_SIZE);
		return;
	}
	cmdBuf[0] = WRITE_ENABLE;
	at25sf161Write(cmdBuf, 1);

	cmdBuf[0] = WRITE_EEPROM;				  // write command.
	cmdBuf[1] = (eepromAddress >> 16) & 0xFF; // address bits 15-23
	cmdBuf[2] = (eepromAddress >> 8) & 0xFF;  // address bits  8-15
	cmdBuf[3] = (eepromAddress)&0xFF;		  // address bits  0-7

	printf("[INFO] Write cmd 0x%x at 0x%02x%02x%02x of length %d bytes.\n", cmdBuf[0], cmdBuf[1], cmdBuf[2], cmdBuf[3], length);

	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.
	// eeprom read command.
	SPI_transmit(currentSpi, cmdBuf[0]);
	SPI_transmit(currentSpi, cmdBuf[1]);
	SPI_transmit(currentSpi, cmdBuf[2]);
	SPI_transmit(currentSpi, cmdBuf[3]);
	// reading data.
	while (length)
	{
		SPI_transmit(currentSpi, *writeBuf);
		writeBuf++;
		length--;
	}
	SPI_set_CSAAT_pin(currentSpi, LOW); // Setting CSAAT bit low.
	at25sf161BusyWait();
}

/** 
 * @fn void at25sf161Read(char *readBuf, int length, char opcode)
 * @brief To read registers of at25sf161 eeprom.
 * @details 
 * @param[in] char *readBuf, int length, char opcode
 * @param[Out] No output parameter 
*/
void at25sf161Read(char *readBuf, int length, char opcode)
{
	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.
	SPI_transmit(currentSpi, opcode);
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
 * @fn void at25sf161Write(char *writeBuf, int length)
 * @brief To write command to at25sf161 eeprom.
 * @details 
 * @param[in] char *writeBuf, int length
 * @param[Out] No output parameter 
*/
void at25sf161Write(char *writeBuf, int length)
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
 * @fn void at25sf161BlockErase(unsigned int eepromAddress, int eraseMode)
 * @brief For at25sf161 eeprom block erase.
 * @details 
 * @param[in] unsigned int eepromAddress, int eraseMode 
 * @param[Out] No output parameter 
*/
void at25sf161BlockErase(unsigned int eepromAddress, int eraseMode)
{
	unsigned char cmdBuf[4];
	SPI_wait_if_busy(currentSpi);			  // Check if SPI controller is busy.
	cmdBuf[1] = (eepromAddress >> 16) & 0xFF; // address bits 15-23
	cmdBuf[2] = (eepromAddress >> 8) & 0xFF;  // address bits  8-15
	cmdBuf[3] = (eepromAddress)&0xFF;		  // address bits  0-7

	cmdBuf[0] = WRITE_ENABLE;
	at25sf161Write(cmdBuf, 1);

	switch (eraseMode)
	{
	case ERASE_4K:
		cmdBuf[0] = BLOCK_ERASE_4K; // Erase opcode
		break;
	case ERASE_32K:
		cmdBuf[0] = BLOCK_ERASE_32K;
		break;
	case ERASE_64K:
		cmdBuf[0] = BLOCK_ERASE_64K;
		break;
	default:
		printf("[EROR] Invalid erase mode (%d).\n", eraseMode);
		return;
	}
	printf("[WARN] Erase cmd 0x%x at 0x%02x%02x%02x.\n", cmdBuf[0], cmdBuf[1], cmdBuf[2], cmdBuf[3]);
	at25sf161Write(cmdBuf, 4);
	at25sf161BusyWait();
}

/** 
 * @fn void at25sf161ChipErase(void)
 * @brief For compleate at25sf161 eeprom erase.
 * @details 
 * @param[in] No output parameter 
 * @param[Out] No output parameter 
*/
void at25sf161ChipErase(void)
{
	static char opcode;
	SPI_wait_if_busy(currentSpi); // Check if SPI controller is busy.
	printf("[WARN] executing full chip erase at spi%d.\n", currentSpi);
	opcode = WRITE_ENABLE;
	at25sf161Write(&opcode, 1);
	opcode = CHIP_ERASE;
	at25sf161Write(&opcode, 1);
	at25sf161BusyWait();
	printf("[INFO] chip erase done at spi%d.\n", currentSpi);
}

/** 
 * @fn void at25sf161BusyWait(void)
 * @brief Wait till at25sf161 eeprom is busy.
 * @details 
 * @param[in] No output parameter 
 * @param[Out] No output parameter 
*/
void at25sf161BusyWait(void)
{
	unsigned char status;

	do
	{
		at25sf161Read(&status, 1, READ_STATUS_1);
	} while (status & EEPROM_BUSY);
}