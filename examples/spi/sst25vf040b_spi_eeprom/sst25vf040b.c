/***************************************************************************
* Module name                           :  main.c
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
#include "sst25vf040b.h"

/**
 @file sst25vf040b.c
 @brief AT25SF161 SPI Flash Library 
 @detail 
 */

unsigned char currentSpi = 0;

/** 
 * @fn void sst25vf040bBegin(unsigned char spiNo) 
 * @brief To initialize the sst25vf040b eeprom.
 * @details 
 * @param[in] unsigned char spiNo
 * @param[Out] No output parameter 
*/
void sst25vf040bBegin(unsigned char spiNo)
{
	static char devID[3], cmdBuf[2];
	SPIcntrlRegType sst25vf040bSpiCtrl;
	unsigned char status;

	// Custom SPI control register setup, refer SPI datasheet for more details.
	sst25vf040bSpiCtrl.Bits.PeriphChipSelect = 0;
	sst25vf040bSpiCtrl.Bits.PeriphType = 0;
	sst25vf040bSpiCtrl.Bits.DataOrder = MSB;
	sst25vf040bSpiCtrl.Bits.Mode = 3;					  // SPI mode 3
	sst25vf040bSpiCtrl.Bits.SPIrxTxIntr = RX_TX_INTR_DIS; // no interrupts
	sst25vf040bSpiCtrl.Bits.CSAAT = LOW;
	sst25vf040bSpiCtrl.Bits.Dbits = 0; // 8-bit SPI operation.

	currentSpi = spiNo;
	SPI_init(currentSpi);
	SPI_config(currentSpi, sst25vf040bSpiCtrl); //modifing spi control register.
	SPI_set_baud(currentSpi, SPI_BAUD_CFD_4);	// setting spi baud rate.
	sst25vf040bRead(devID, 3, READ_DEV_ID);
	printf("[INFO] eeprom %02x:%02x:%02x initialized at spi%d.\n", devID[0], devID[1], devID[2], currentSpi);

	/* Setting Status register */
	status = DISABLE_BLOCK_PROTECTION;

	cmdBuf[0] = ENABLE_STATUS_WRITE;
	sst25vf040bWrite(cmdBuf, 1);

	cmdBuf[0] = WRITE_STATUS;
	cmdBuf[1] = status;
	sst25vf040bWrite(cmdBuf, 2);
}

/** 
 * @fn void sst25vf040bReadEeprom(char *readBuf, int length, unsigned int eepromAddress)
 * @brief To read sst25vf040b eeprom data.
 * @details 
 * @param[in] char *readBuf, int length, unsigned int eepromAddress
 * @param[Out] No output parameter 
*/
void sst25vf040bReadEeprom(char *readBuf, int length, unsigned int eepromAddress)
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
 * @fn void sst25vf040bWriteEeprom(char *writeBuf, int length, unsigned int eepromAddress)
 * @brief To write data to sst25vf040b eeprom.
 * @details 
 * @warning Min Write length should not be less than 2 and length should be an even number.
 * @param[in] char *writeBuf, int length, unsigned int eepromAddress
 * @param[Out] No output parameter 
*/
void sst25vf040bWriteEeprom(char *writeBuf, int length, unsigned int eepromAddress)
{
	char cmdBuf[6];

	if (length < MIN_LENGTH)
	{
		printf("[EROR] Write length is less than minimum length (%d).\n", MIN_LENGTH);
		return;
	}

	if ((length % 2) != 0)
	{
		printf("[WARN] Write length is not an even number.Last byte will be ommited.\n");
		length--;
	}

	cmdBuf[0] = WRITE_ENABLE;
	sst25vf040bWrite(cmdBuf, 1);

	cmdBuf[0] = WRITE_EEPROM;				  // write command.
	cmdBuf[1] = (eepromAddress >> 16) & 0xFF; // address bits 15-23
	cmdBuf[2] = (eepromAddress >> 8) & 0xFF;  // address bits  8-15
	cmdBuf[3] = (eepromAddress)&0xFF;		  // address bits  0-7
	cmdBuf[4] = *writeBuf;					  // data  0
	writeBuf++;
	length--;
	cmdBuf[5] = *writeBuf; // data  1
	writeBuf++;
	length--;
	printf("[INFO] Write cmd 0x%x at 0x%02x%02x%02x of length %d bytes.\n", cmdBuf[0], cmdBuf[1], cmdBuf[2], cmdBuf[3], length);
	sst25vf040bWrite(cmdBuf, 6);
	while (length)
	{
		sst25vf040bBusyWait();
		cmdBuf[1] = *writeBuf; // data  0
		writeBuf++;
		length--;
		cmdBuf[2] = *writeBuf; // data  1
		writeBuf++;
		length--;
		sst25vf040bWrite(cmdBuf, 3);
	}
	sst25vf040bBusyWait();
	cmdBuf[0] = WRITE_DISABLE;
	sst25vf040bWrite(cmdBuf, 1);
}

/** 
 * @fn void sst25vf040bRead(char *readBuf, int length, char opcode)
 * @brief To read registers of sst25vf040b eeprom.
 * @details 
 * @param[in] char *readBuf, int length, char opcode
 * @param[Out] No output parameter 
*/
void sst25vf040bRead(char *readBuf, int length, char opcode)
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
 * @fn void sst25vf040bWrite(char *writeBuf, int length)
 * @brief To write command to sst25vf040b eeprom.
 * @details 
 * @param[in] char *writeBuf, int length
 * @param[Out] No output parameter 
*/
void sst25vf040bWrite(char *writeBuf, int length)
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
 * @fn void sst25vf040bBlockErase(unsigned int eepromAddress, int eraseMode)
 * @brief For sst25vf040b eeprom block erase.
 * @details 
 * @param[in] unsigned int eepromAddress, int eraseMode 
 * @param[Out] No output parameter 
*/
void sst25vf040bBlockErase(unsigned int eepromAddress, int eraseMode)
{
	unsigned char cmdBuf[4];
	SPI_wait_if_busy(currentSpi);			  // Check if SPI controller is busy.
	cmdBuf[1] = (eepromAddress >> 16) & 0xFF; // address bits 15-23
	cmdBuf[2] = (eepromAddress >> 8) & 0xFF;  // address bits  8-15
	cmdBuf[3] = (eepromAddress)&0xFF;		  // address bits  0-7

	cmdBuf[0] = WRITE_ENABLE;
	sst25vf040bWrite(cmdBuf, 1);

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
	sst25vf040bWrite(cmdBuf, 4);
	sst25vf040bBusyWait();
}

/** 
 * @fn void sst25vf040bChipErase(void)
 * @brief For compleate sst25vf040b eeprom erase.
 * @details 
 * @param[in] No output parameter 
 * @param[Out] No output parameter 
*/
void sst25vf040bChipErase(void)
{
	static char opcode;

	SPI_wait_if_busy(currentSpi); // Check if SPI controller is busy.
	printf("[WARN] executing full chip erase at spi%d.\n", currentSpi);
	opcode = WRITE_ENABLE;
	sst25vf040bWrite(&opcode, 1);
	opcode = CHIP_ERASE;
	sst25vf040bWrite(&opcode, 1);
	sst25vf040bBusyWait();
	printf("[INFO] chip erase done at spi%d.\n", currentSpi);
}

/** 
 * @fn void sst25vf040bBusyWait(void)
 * @brief Wait till sst25vf040b eeprom is busy.
 * @details 
 * @param[in] No output parameter 
 * @param[Out] No output parameter 
*/
void sst25vf040bBusyWait(void)
{
	unsigned char status;

	do
	{
		sst25vf040bRead(&status, 1, READ_STATUS);
	} while (status & EEPROM_BUSY);
}