#ifndef DS18b20_H_
#define DS18b20_H_

#include "stdlib.h"
#include "config.h"

/* In case of porting this library, do work on 'delay_1_us' and make sure that all delays are accurate.*/

#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0
#define ONE_WIRE_GPIO 1			// Max 0 - 15 in Thejas SoC.
#define DS18B20_NOT_FOUND 40960 // Invalid temp data.
#define TEMP_DATA_MASK 0xFFF 	// only 12 bits are valid for ds18b20.
#define TEMP_SIGN_BIT  (1 << 11) // 11 th bit is sign bit.

//Defining ROM commands
#define DS18S20_SKIP_ROM 0xCC
#define DS18S20_READ_SCRATCHPAD 0xBE
#define DS18S20_CONVERT_T 0x44

/* 1 micro second delay calculated for Thejas32 40Mhz processor.*/
static inline void delay_1_us(void)
{
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
}

/* Functions for Thejas32 1-wire GPIO */
void setWireInput(void);
void setWireOutput(void);
void writeWire(US bit);
US readWire(void);

/* Functions for DS18B20 temp sensor */
void ds18b20Init(void);
unsigned int ds18b20Reset(void);
void ds18b20High(void);
void ds18b20Low(void);
void writeWireZero(void);
void writeWireOne(void);
unsigned int readWireBit(void);
void write_ds18b20(unsigned char);
unsigned int read_ds18b20(void);
int getRawTemperature(void);

#endif /*DS18S20_H_*/
