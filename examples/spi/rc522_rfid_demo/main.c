/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 27-july-2021
 Filename		: main.c
 Purpose		: spi mfrc522 demo
 Description	: Sample RC522 RFid reader with spi interface
 Author(s)		: Thomas Sebastian, C-DAC
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @brief RC522 RFid reader demo.
 @details interface RC522 RFid reader module with  spi .
 @param[in] No input parameter. 
 @param[Out] No output parameter. 
*/

#include "stdlib.h"
#include "spi.h"
#include "mfrc522.h"

uint8_t SelfTestBuffer[64];
UC data[8];
int indx = 0;

void main()
{
	uint8_t byte;
	uint8_t str[MAX_LEN];

	mfrc522_init(0);

	for (size_t i = 0; i < 0x3B; i++)
	{
		byte = mfrc522_read(i);
		//printf("REG %X DATA : %x\n",i, byte);
	}

	/*  // CODE FOR DETECTING THE VERSION OF READER MODULE

	byte = mfrc522_read(VersionReg);
	printf("Version no : %x\n",byte);

	if(byte == 0x92)
	{
		printf("MIFARE RC522v2");
		printf("Detected");
	}else if(byte == 0x91 || byte==0x90)
	{
		printf("MIFARE RC522v1");
		printf("Detected");
	}else
	{
		printf("\nNo reader found");
	}
*/
	byte = mfrc522_read(ComIEnReg);
	mfrc522_write(ComIEnReg, byte | 0x20);
	byte = mfrc522_read(DivIEnReg);
	mfrc522_write(DivIEnReg, byte | 0x80);

	udelay(1500);

	while (1)
	{

		byte = mfrc522_request(PICC_REQALL, str);

		if (byte == CARD_FOUND)
		{
			byte = mfrc522_get_card_serial(str);

			if (byte == CARD_FOUND)
			{
				printf("\n card ID : %02x %02x %02x %02x", str[0], str[1], str[2], str[3]);

				switch (str[0])
				{
				case 0x43: // 0x43 -- initial byte of card 1 ,, change according to each rfid card
					printf("\n card type 1");
					break;
				case 0xe3: // 0xe3 -- initial byte of card 2 ,, change according to each rfid card
					printf("\n card type 2");
					break;
				default:
					printf("\n card type unknown");
					break;
				}
				udelay(25000);
				printf("\n");
			}
			else
			{
				printf("Error");
			}
		}

		udelay(10000);
	}
}
