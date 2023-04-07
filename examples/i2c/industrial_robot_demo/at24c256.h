#ifndef AT24C256_H_
#define AT24C256_H_

#define AT24C256_EEPROM_I2C_ADDRESS 0x50

void at24c256Begin(unsigned char i2cNo);
void at24c256Write(char *writeBuf, int length, unsigned short eepromAddress);
void at24c256Read(char *readBuf, int length, unsigned short eepromAddress);

#endif