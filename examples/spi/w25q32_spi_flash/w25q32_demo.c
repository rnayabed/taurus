#include "stdlib.h"
#include "spi.h"
#include "w25q32.h"

/**
 @brief W25Q32 flash demo.
 @details Read, write and erase W25Q32 flash.
 @param[in] No input parameter. 
 @param[Out] No output parameter. 
 */
void main() {
	UL rand_value = 99;
	static char writeData[256], readData[256];

	printf("+------[ W25Q32 Demo ]------+\n");
	printf("|                                 |\n");
	printf("|     W25Q32  ----> SPI0     |\n");
	printf("|                                 |\n");
	printf("+---------------------------------+\n\n");

	w25q32Begin(MDP_SPI_0);

	w25q32ChipErase(); // Complete erase chip

	w25q32ReadFlash(readData, 256, 0x0); // read first 256 bytes of data.
	printf("\n w25q32 read data after erase:");
	for (int i = 0; i < 256; i++) {
		if (i % 50 == 0)
			printf("\n");
		printf("%02x ", readData[i]);
	}
	printf("\n");
	printf("WARN: No: of bytes to be written should be less than 256.\n");
	// generating random data.

	for (int i = 0; i < 255; i++) {
		rand_value = rand_value * 5;
		if (rand_value == 0)
			rand_value = 99;
		writeData[i] = (UC) rand_value;
		//writeData[i] = i % 256;


	}

	w25q32WriteFlash(writeData, 255, 0x00); // writing 255 bytes of data .
	w25q32ReadFlash(readData, 255, 0x0);	 // read first 255 bytes of data.
	printf("\n w25q32 read data after write:");
	for (int i = 0; i < 255; i++) {
		if (i % 50 == 0)
			printf("\n");
		printf("%02x ", readData[i]);
	}
	printf("\n");

	while (1)
		;
}
