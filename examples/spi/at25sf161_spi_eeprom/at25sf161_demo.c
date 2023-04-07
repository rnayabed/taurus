#include "stdlib.h"
#include "spi.h"
#include "at25sf161.h"

/**
 @brief AT25SF161 eeprom demo.
 @details Read, write and erase AT25SF161 eeprom.
 @param[in] No input parameter. 
 @param[Out] No output parameter. 
*/
void main()
{
	static char writeData[256], readData[512];

	printf("+------[ AT25SF161  Demo ]------+\n");
	printf("|                               |\n");
	printf("|     AT25SF161  ----> SPI0     |\n");
	printf("|                               |\n");
	printf("+-------------------------------+\n\n");

	at25sf161Begin(MDP_SPI_0);
	//at25sf161ChipErase(); // Complete erase chip

	//at25sf161ReadEeprom(readData, 512, 0x0); // read first 512 bytes of data.
	printf("\n at25sf161 read data :");
	for (int i = 0; i < 512; i++)
	{
		if (i % 32 == 0)
			printf("\n");
		printf("%02x ", readData[i]);
	}
	printf("\n");
	// generating data.
	for (int i = 0; i < 128; i++)
	{
		writeData[i] = i;
	}
	at25sf161WriteEeprom(writeData, 128, 0x0);	 // writing data in page 1. Max length for write is 256 (page size).
	at25sf161WriteEeprom(writeData, 128, 0x100); // writing data in page 2.

	at25sf161ReadEeprom(readData, 512, 0x0); // read first 512 bytes of data.
	printf("\n at25sf161 read data :");
	for (int i = 0; i < 512; i++)
	{
		if (i % 32 == 0)
			printf("\n");
		printf("%02x ", readData[i]);
	}
	printf("\n");
	//at25sf161BlockErase(0x0, ERASE_64K); // Erase first 64k from flash.
	while (1)
		;
}
