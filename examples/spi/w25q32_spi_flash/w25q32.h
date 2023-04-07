#ifndef _W25Q32_H
#define _W25Q32_H

#define HIGH 1
#define LOW 0

/* opcodes */
#define READ_DEV_ID 0x9F         // JEDEC ID read
#define READ_FLASH 0x03         // Read Memory
#define PAGE_PROGRAM 0x02        // Page Programming
#define CHIP_ERASE 0x60          // Erase Full Memory Array
#define READ_STATUS 0x05         // Read-Status-Register
#define WRITE_ENABLE 0x06        // Write-Enable
#define WRITE_DISABLE 0x04       // Write-Disable
#define WRITE_STATUS 0x01        // Write-Status-Register
#define ENABLE_STATUS_WRITE 0x50 // Enable-Write-Status-Register
#define BLOCK_ERASE_4K 0x20      // Erase 4 KByte of memory array
#define BLOCK_ERASE_32K 0x52     // Erase 32 KByte block of memory array
#define BLOCK_ERASE_64K 0xD8     // Erase 64 KByte block of memory array

#define DUMMY_DATA 0x00
#define EEPROM_BUSY 0x01
#define MIN_LENGTH 2
#define ERASE_4K 1
#define ERASE_32K 2
#define ERASE_64K 3
#define DISABLE_BLOCK_PROTECTION 0x80 // BLOCK PROTECTION LOCK-DOWN




void w25q32Begin(unsigned char spiNo);
void w25q32Read(char *readBuf, int length, char opcode);
void w25q32ChipErase(void);
void w25q32Write(char *writeBuf, int length);
void w25q32ReadFlash(char *readBuf, int length, unsigned int flashAddress);
void w25q32WriteFlash(char *writeBuf, int length, unsigned int flashAddress);
void w25q32BusyWait(void);

#endif /* _SST25VF040B_H */
