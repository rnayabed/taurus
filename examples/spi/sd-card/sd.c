/***************************************************************************
* Module name                           :  sd.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  SD-Card SPI Library

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
#include <stdint.h>
#include "stdlib.h"
#include "spi.h"
#include "sd.h"

/**
 @file at25sf161.c
 @brief AT25SF161 SPI Flash Library 
 @detail 
 */

unsigned char currentSpi = 0;

int sdInit(unsigned char spiNo)
{
	static char response;
	SPIcntrlRegType at25sf161SpiCtrl;

	// Custom SPI control register setup, refer SPI datasheet for more details.
	at25sf161SpiCtrl.Bits.PeriphChipSelect = 0;
	at25sf161SpiCtrl.Bits.PeriphType = 0;
	at25sf161SpiCtrl.Bits.DataOrder = MSB;
	at25sf161SpiCtrl.Bits.Mode = 0;						// SPI mode 3
	at25sf161SpiCtrl.Bits.SPIrxTxIntr = RX_TX_INTR_DIS; // no interrupts
	at25sf161SpiCtrl.Bits.CSAAT = LOW;
	at25sf161SpiCtrl.Bits.Dbits = 0; // 8-bit SPI operation.

	currentSpi = spiNo;
	SPI_init(currentSpi);
	SPI_config(currentSpi, at25sf161SpiCtrl); // modifing spi control register.
	SPI_set_baud(currentSpi, SPI_BAUD_CFD_4); // setting spi baud rate.
	//Prior to the initialization, the card must know the frequency at which the data is being sent.
	// By sending around 75 dummy bits approximately (dummy byte * 10 times = 80 bits), the card will be ready for communication.
	sendDummy(75);
	LOG("[...] Check for SD idle (CMD0)\n", response);
	response = sendCommand(CMD0, CMD0_ARGS, CMD0_CRC);
	if (response != CARD_IDLE)
	{
		return SD_ERROR;
	}
	LOG("[...] Check for SD Version (CMD8)\n", response);
	response = sendCommand(CMD8, CMD8_ARGS, CMD8_CRC);
	switch (response)
	{
	case SD_VERSION_1:
		printf("SD card version 1\n");
		break;

	case SD_VERSION_2:
		printf("SD card version 2\n");
		break;

	default:
		printf("SD card version unknown !\n");
		break;
	}
	do
	{
		LOG("[...] Card initialization (CMD1)\n");
		response = sendCommand(CMD1, CMD1_ARGS, CMD1_CRC);
	} while (response != CMD1_OK);
	do
	{
		LOG("[...] Set the block size of the read/write block (CMD16)\n");
		response = sendCommand(CMD16, CMD16_ARGS, CMD16_CRC);
	} while (response != CMD16_OK);
	return 0;
}

void sendDummy(int dummys)
{
	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.
	while (dummys)
	{
		SPI_transmit(currentSpi, DUMMY_DATA);
		dummys--;
	}

	SPI_set_CSAAT_pin(currentSpi, LOW); // Setting CSAAT bit low.
}

uint8_t sendCommand(uint8_t command, uint32_t arguments, uint8_t crc)
{
	uint8_t command_buf[6], response = 0xFF;
	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.

	command_buf[0] = command | COMMAND_START;
	command_buf[1] = (arguments >> 24) & 0xFF;
	command_buf[2] = (arguments >> 16) & 0xFF;
	command_buf[3] = (arguments >> 8) & 0xFF;
	command_buf[4] = arguments & 0xFF;
	command_buf[5] = crc;
	SPI_transmit(currentSpi, DUMMY_DATA);
	for (int i = 0; i < 6; i++)
	{
		SPI_transmit(currentSpi, command_buf[i]);
	}
	while (response & R1_MASK)
	{
		SPI_transmit(currentSpi, DUMMY_DATA);
		response = SPI_receive(currentSpi); //data
	}
	/* reading command responce data.. not using now reserved for future */
	LOG("[...] Response : %x \n", response);
	SPI_transmit(currentSpi, DUMMY_DATA);
	LOG("[...] Response : %x \n", SPI_receive(currentSpi) & 0xff);
	SPI_transmit(currentSpi, DUMMY_DATA);
	LOG("[...] Response : %x \n", SPI_receive(currentSpi) & 0xff);
	SPI_transmit(currentSpi, DUMMY_DATA);
	LOG("[...] Response : %x \n", SPI_receive(currentSpi) & 0xff);
	SPI_transmit(currentSpi, DUMMY_DATA);
	SPI_set_CSAAT_pin(currentSpi, LOW); // Setting CSAAT bit low.
	return response;
}

int readBlock(uint32_t address, uint8_t * write_buf)
{
	uint8_t command_buf[6], response = 0xFF;

	LOG("[...] Single read block (CMD17) at 0x%08x\n", address);
	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.

	command_buf[0] = CMD17 | COMMAND_START;
	command_buf[1] = (address >> 24) & 0xFF;
	command_buf[2] = (address >> 16) & 0xFF;
	command_buf[3] = (address >> 8) & 0xFF;
	command_buf[4] = address & 0xFF;
	command_buf[5] = DUMMY_DATA;
	SPI_transmit(currentSpi, DUMMY_DATA);
	for (int i = 0; i < 6; i++)
	{
		SPI_transmit(currentSpi, command_buf[i]);
	}
	while (response & R1_MASK)
	{
		SPI_transmit(currentSpi, DUMMY_DATA);
		response = SPI_receive(currentSpi); //data
	}
	LOG("[...] Response : %x \n", response);
	if (response != CMD17_OK)
	{
		LOG("[...] CMD17 failed !\n");
		SPI_set_CSAAT_pin(currentSpi, LOW);
		return SD_ERROR;
	}
	do
	{
		SPI_transmit(currentSpi, DUMMY_DATA);
		response = SPI_receive(currentSpi); //data
	} while (response != SD_READ_READY);
	LOG("[...] Card read started.\n");
	for (int i = 0; i < BYTES_512; i++)
	{
		SPI_transmit(currentSpi, DUMMY_DATA);
		response = SPI_receive(currentSpi); //data
		*write_buf = response;
		write_buf++;
		//if (i % 16 == 0)
		//	printf("\n");
		//printf(" %02x", response);
	}
	//printf("\n");
	/* send a dummy byte or two and the process is complete. */
	SPI_transmit(currentSpi, DUMMY_DATA);
	SPI_transmit(currentSpi, DUMMY_DATA);
	SPI_set_CSAAT_pin(currentSpi, LOW);
	return 0;
}
int readMultiBlock(uint32_t address, uint32_t nBlocks, uint8_t * write_buf)
{
	uint8_t command_buf[6], response = 0xFF;

	LOG("[...] Single read block (CMD18) at 0x%08x\n", address);
	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.

	command_buf[0] = CMD18 | COMMAND_START;
	command_buf[1] = (address >> 24) & 0xFF;
	command_buf[2] = (address >> 16) & 0xFF;
	command_buf[3] = (address >> 8) & 0xFF;
	command_buf[4] = address & 0xFF;
	command_buf[5] = DUMMY_DATA;
	SPI_transmit(currentSpi, DUMMY_DATA);
	for (int i = 0; i < 6; i++)
	{
		SPI_transmit(currentSpi, command_buf[i]);
	}
	while (response & R1_MASK)
	{
		SPI_transmit(currentSpi, DUMMY_DATA);
		response = SPI_receive(currentSpi); //data
	}
	LOG("[...] Response : %x \n", response);
	if (response != CMD18_OK)
	{
		LOG("[...] CMD18 failed !\n");
		SPI_set_CSAAT_pin(currentSpi, LOW);
		return SD_ERROR;
	}
	do
	{
		SPI_transmit(currentSpi, DUMMY_DATA);
		response = SPI_receive(currentSpi); //data
	} while (response != SD_READ_READY);
	LOG("[...] Card read started.\n");
	while (nBlocks)
	{
		for (int i = 0; i < BYTES_512; i++)
		{
			SPI_transmit(currentSpi, DUMMY_DATA);
			response = SPI_receive(currentSpi); //data
		//	if (i % 16 == 0)
		//		printf("\n");
		//	printf(" %02x", response);
			*write_buf = response;
			write_buf++;
		}
		//printf("\n");
		nBlocks--;
	}

	/* Tell card read over */
	command_buf[0] = CMD12 | COMMAND_START;
	command_buf[1] = 0;
	command_buf[2] = 0;
	command_buf[3] = 0;
	command_buf[4] = 0;
	command_buf[5] = DUMMY_DATA;

	for (int i = 0; i < 6; i++)
	{
		SPI_transmit(currentSpi, command_buf[i]);
	}

	/* send a dummy byte or two and the process is complete. */
	SPI_transmit(currentSpi, DUMMY_DATA);
	SPI_transmit(currentSpi, DUMMY_DATA);
	SPI_set_CSAAT_pin(currentSpi, LOW);
	return 0;
}
void waitTillDie(void)
{
	printf("Program execution over!\n");
	while (1)
		;
}
/*
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
*/