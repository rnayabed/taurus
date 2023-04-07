#include "stdlib.h"
#include "spi.h"
#include "sst25vf040b.h"

/**
 @brief SST25VF040B eeprom demo.
 @details Read, write and erase SST25VF040B eeprom.
 @param[in] No input parameter. 
 @param[Out] No output parameter. 
*/
void main()
{
	static char writeData[512], readData[512];

	printf("+------[ SST25VF040B  Demo ]------+\n");
	printf("|                                 |\n");
	printf("|     SST25VF040B  ----> SPI0     |\n");
	printf("|                                 |\n");
	printf("+---------------------------------+\n\n");

	sst25vf040bBegin(MDP_SPI_0);
	sst25vf040bChipErase(); // Complete erase chip

	sst25vf040bReadEeprom(readData, 512, 0x0); // read first 512 bytes of data.
	printf("\n sst25vf040b read data :");
	for (int i = 0; i < 512; i++)
	{
		if (i % 32 == 0)
			printf("\n");
		printf("%02x ", readData[i]);
	}
	printf("\n");

	// generating data.
	for (int i = 0; i < 512; i++)
	{
		writeData[i] = i % 256;
	}
	sst25vf040bWriteEeprom(writeData, 512, 0x0); // writing 512 bytes of data .
	sst25vf040bReadEeprom(readData, 512, 0x0);	 // read first 512 bytes of data.
	printf("\n sst25vf040b read data :");
	for (int i = 0; i < 512; i++)
	{
		if (i % 32 == 0)
			printf("\n");
		printf("%02x ", readData[i]);
	}
	printf("\n");
	sst25vf040bBlockErase(0x0, ERASE_64K); // Erase first 64k from flash.
	while (1)
		;
}
