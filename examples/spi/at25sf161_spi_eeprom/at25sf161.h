#ifndef _ATS25SF161_H
#define _ATS25SF161_H

#define HIGH 1
#define LOW 0

/* opcodes */
#define READ_DEV_ID 0x9F     // Read Manufacturer and Device ID
#define READ_EEPROM 0x03     // Read Array
#define WRITE_EEPROM 0x02    // Byte/Page Program
#define CHIP_ERASE 0x60      // Chip Erase
#define READ_STATUS_1 0x05   // Read Status Register - Byte 1
#define READ_STATUS_2 0x35   // Read Status Register - Byte 2
#define WRITE_ENABLE 0x06    // Write Enable
#define BLOCK_ERASE_4K 0x20  // Block Erase (4 Kbytes)
#define BLOCK_ERASE_32K 0x52 // Block Erase (32 Kbytes)
#define BLOCK_ERASE_64K 0xD8 // Block Erase (64 Kbytes)

#define DUMMY_DATA 0x00
#define EEPROM_BUSY 0x01
#define PAGE_SIZE 256
#define ERASE_4K 1
#define ERASE_32K 2
#define ERASE_64K 3

void at25sf161Begin(unsigned char spi_no);
void at25sf161Read(char *readBuff, int length, char opcode);
void at25sf161Write(char *writeBuf, int length);
void at25sf161ReadEeprom(char *readBuf, int length, unsigned int eepromAddress);
void at25sf161WriteEeprom(char *writeBuf, int length, unsigned int eepromAddress);
void at25sf161BlockErase(unsigned int eepromAddress, int eraseMode);
void at25sf161ChipErase(void);
void at25sf161BusyWait(void);

#endif /* _ADS25SF161_H */