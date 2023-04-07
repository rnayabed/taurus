#ifndef _SST25VF040B_H
#define _SST25VF040B_H

#define HIGH 1
#define LOW 0

/* opcodes */
#define READ_DEV_ID 0x9F         // JEDEC ID read
#define READ_EEPROM 0x03         // Read Memory
#define WRITE_EEPROM 0xAD        // Auto Address Increment Programming
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
#define AUTO_ADDRESS_INCREMENT 0x40   // Auto Address Increment Programming status

void sst25vf040bBegin(unsigned char spi_no);
void sst25vf040bRead(char *readBuff, int length, char opcode);
void sst25vf040bWrite(char *writeBuf, int length);
void sst25vf040bReadEeprom(char *readBuf, int length, unsigned int eepromAddress);
void sst25vf040bWriteEeprom(char *writeBuf, int length, unsigned int eepromAddress);
void sst25vf040bBlockErase(unsigned int eepromAddress, int eraseMode);
void sst25vf040bChipErase(void);
void sst25vf040bBusyWait(void);

#endif /* _SST25VF040B_H */