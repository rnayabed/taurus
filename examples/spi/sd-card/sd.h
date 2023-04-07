#ifndef _SD_H
#define _SD_H

/* Loging */
#define LOGING
#ifdef LOGING
#define LOG(fmt, args...)    printf(fmt, ## args)
#else
#define LOG(fmt, args...)    /* Don't do anything in release builds */
#endif

/* sd-card commands */
#define COMMAND_START 0x40 //The first byte is the addition of the command number and the number 0x40.
#define CMD0 0 // Software reset.
#define CMD0_ARGS 0x0
#define CMD0_CRC 0x95

#define CMD8 8
#define CMD8_ARGS 0x000001AA
#define CMD8_CRC 0x87

#define CMD1 1 // Software reset.
#define CMD1_ARGS 0x40000000
#define CMD1_CRC 0x77

#define CMD16 16 // specify the block length.
#define CMD16_ARGS BYTES_512
#define CMD16_CRC 0x81

#define CMD17 17 //single read block
#define CMD18 18 //single read block
#define CMD12 12

/* Response R1 */
#define CARD_IDLE 0x1

/* SD support */
#define SD_ERROR -99
#define R1_MASK 0x80
#define SD_VERSION_1 0x5
#define SD_VERSION_2 0x1
#define CMD1_OK 0x0
#define CMD16_OK 0x0
#define CMD17_OK 0x0
#define CMD18_OK 0x0
#define SD_READ_READY 0xFE
#define BYTES_512  0x00000200


/* SPI support */
#define DUMMY_DATA 0xFF
#define HIGH 1
#define LOW 0


int sdInit(unsigned char spiNo);
uint8_t sendCommand(uint8_t command, uint32_t arguments, uint8_t crc);
void sendDummy(int dummys);
int readBlock(uint32_t address, uint8_t * write_buf);
int readMultiBlock(uint32_t address, uint32_t nBlocks, uint8_t * write_buf);
void waitTillDie(void);
#endif /* _SD_H */