/***************************************************************************
* Module name                           :  max7219.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  max7219 SPI Flash Library

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
#include "max7219.h"

/**
 @file max7219.c
 @brief max7219 SPI Flash Library 
 @detail 
 */
// hexcode for each alphabets.
unsigned long long ALHABETS[] = {

	0x2222223e22221c00,
	0x1e22221e22221e00,
	0x3804020202043800,
	0x0e12222222120e00,
	0x3e02021e02023e00,
	0x0202021e02023e00,
	0x0c2a3a0202241800,
	0x2222223e22222200,
	0x3e08080808083e00,
	0x1e12101010103e00,
	0x22120a060a122200,
	0x3e22020202020200,
	0x2222222a2a2a1400,
	0x2232322a26262200,
	0x1c22222222221c00,
	0x0202021e22221e00,
	0x2c122a2622221c00,
	0x22120a1e22221e00,
	0x1c22201c02221c00,
	0x08080808082a3e00,
	0x1c22222222222200,
	0x0814222222222200,
	0x142a2a2a22222200,
	0x2222140808142200,
	0x0808080814222200,
	0x3e02040810203e00,
	0x0000000000000000};
unsigned char currentSpi = 0;

/** 
 * @fn void max7219Begin(unsigned char spiNo) 
 * @brief To initialize the max7219 .
 * @details 
 * @param[in] unsigned char spiNo
 * @param[Out] No output parameter 
*/
void max7219Begin(unsigned char spiNo)
{
	SPIcntrlRegType max7219SpiCtrl;
	unsigned short data;

	// Custom SPI control register setup, refer SPI datasheet for more details.
	max7219SpiCtrl.Bits.PeriphChipSelect = 0;
	max7219SpiCtrl.Bits.PeriphType = 0;
	max7219SpiCtrl.Bits.DataOrder = MSB;
	max7219SpiCtrl.Bits.Mode = 3;					  // SPI mode 3
	max7219SpiCtrl.Bits.SPIrxTxIntr = RX_TX_INTR_DIS; // no interrupts
	max7219SpiCtrl.Bits.CSAAT = LOW;
	max7219SpiCtrl.Bits.Dbits = 8; // 16-bit SPI operation.

	currentSpi = spiNo;
	SPI_init(currentSpi);
	SPI_config(currentSpi, max7219SpiCtrl);	  // modifing spi control register.
	SPI_set_baud(currentSpi, SPI_BAUD_CFD_16); // setting spi baud rate.

	max7219_test();

	/* setting scan limit */
	data = (SCANLIMIT << 8) | 0x7;
	max7219Write(data);

	/* shut down command */
	data = (SHUTDWN << 8) | 0x1;
	max7219Write(data);

	/* decode command */
	data = (DECODE_MODE << 8) | 0x00;
	max7219Write(data);
}

/** 
 * @fn void max7219Write(char *writeBuf, int length)
 * @brief To write command to max7219 eeprom.
 * @details 
 * @param[in] char *writeBuf, int length
 * @param[Out] No output parameter 
*/

void max7219_test()
{
	unsigned short data;
	/* testing */
	data = (DISPLAY_TEST << 8) | FULL_BRIGHT;
	max7219Write(data);
	udelay(5000);
	data = (DISPLAY_TEST << 8) | FULL_DIM;
	max7219Write(data);
}

void max7219_Brightness(short writeData)
{
	unsigned short data;
	/* Brightness command */
	data = (INTENSITY << 8) | writeData;
	max7219Write(data);
}

void max7219Write(short writeData)
{
	SPI_wait_if_busy(currentSpi);		 // Check if SPI controller is busy.
	SPI_set_CSAAT_pin(currentSpi, HIGH); // Setting CSAAT bit high.
	SPI_transmit(currentSpi, writeData);
	SPI_set_CSAAT_pin(currentSpi, LOW); // Setting CSAAT bit low.
}
void max7219PrintScreen(char *writeBuff)
{
	unsigned short data, bufferData;
	for (int i = 0; i < 8; i++)
	{
		bufferData = 0;
		/* reversing bits to make ardino compactable */
		for (int j = 0; j < 8; j++)
		{
			bufferData = bufferData << 1;
			bufferData |= ((writeBuff[i] >> j) & 0x1);
		}
		data = ((i + 1) << 8) | (bufferData & 0xFF);
		max7219Write(data);
	}
}
// for transition of letters
void max7219Trasistion(unsigned long long char1, unsigned long long char2, int delay)
{
	unsigned long long charBuff;
	unsigned char *char1ptr, *char2ptr, *buffptr;
	for (int i = 0; i < 7; i++)
	{
		charBuff = 0;
		char1ptr = (unsigned char *)&char1;
		char2ptr = (unsigned char *)&char2;
		buffptr = (unsigned char *)&charBuff;
		for (int j = 0; j < 8; j++)
		{
			buffptr[j] = (char1ptr[j] >> i) | (char2ptr[j] << (7 - i));
		}
		udelay(delay);
		max7219PrintScreen((unsigned char *)&charBuff);
	}
}

void max7219ScrollText(char *textBuff, int delay)
{
	int index1, index2;
	int i = 0;
	//for (int i = 0; i < (length - 1); i++)
	while(textBuff[i])
	{	
		index1 = ((textBuff[i] - 'A' < 26) && (textBuff[i] - 'A' >= 0)) ? textBuff[i] - 'A' : 26;			  /* Finding first index */
		index2 = ((textBuff[i + 1] - 'A' < 26) && (textBuff[i + 1] - 'A' >= 0)) ? textBuff[i + 1] - 'A' : 26; /* Finding second index */
		max7219Trasistion(ALHABETS[index1], ALHABETS[index2], delay);
		i++;		
	}
}

void max7219ClearScreen()
{
	unsigned short data;
	for (int i = 0; i < 8; i++)
	{
		data = ((i + 1) << 8) | 0x0;
		max7219Write(data);
	}
	
}



