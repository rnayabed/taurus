#ifndef _QSPI_H
#define _QSPI_H
/**
 @file qspi.h
 @brief header file for QSPI Flash driver
 @detail 
 */
/***************************************************
 * Module name: qspi.h
 *
 * Copyright 2020 Company CDAC(T).
 * All Rights Reserved.
 *
 *  The information contained herein is confidential
 * property of Company. The user, copying, transfer or
 * disclosure of such information is prohibited except
 * by express written agreement with Company.
 *
 *
 * Module Description:
 * QSPI registers and function declarations
 *
 ***************************************************/

/*  Include section
 *
 ***************************************************/

/*  Define section
 *
 *
 ***************************************************/
#if __riscv_xlen == 64 //QSPI driver available only for thejas64
#define QSPI_0			0
#define QSPI_1			1
#define QSPI_2			2
#define QSPI_3			3


#define PRESCALAR_MAX	00
#define PRESCALAR_MIN	0xFF

#define CK_MODE_0         		0
#define CK_MODE_1         		1

#define CHIP_HIGH_TIME 			0x05

#define INSTR_LINE_1	0x01

#define INSTR_LINE_3	0x03


#define POLLING_INTERVAL_256	0x40
#define AUTO_POLL_STOP_ON		0x01
#define AUTO_POLL_STOP_OFF		0x00
#define POLL_MODE_AND			0X00
#define POLL_MODE_OR			0X01


#define DATA_LINE_1		0x01

#define DATA_LINE_3		0x03

#define ADDR_LINE_1		0x01

#define ADDR_LINE_3		0x03

#define DATA_SIZE_0		0x00
#define DATA_SIZE_1		0x01
#define DATA_SIZE_2		0x02
#define DATA_SIZE_3		0x03

#define ADDR_SIZE_0		0x00
#define ADDR_SIZE_1		0x01
#define ADDR_SIZE_2		0x02
#define ADDR_SIZE_3		0x03


#define INDIRECT_WR 	0x00
#define INDIRECT_RD 	0x01
#define AUTOPOLL_RD 	0X02






#define QSPI_BASE_ADDR				0x10200100UL

/*************Micron M25P80 Serial Flash************* 
 *************Embedded Memory Command Set************/
#define RD_STATUS1_REG_QSPI_CMD 	0x05
#define RD_FLASH_ID_CMD 			0x9F
#define RESET_ENABLE_CMD			0x66
#define RESET_FLASH_CMD				0x99
#define WR_EN_LATCH_FLASH_CMD     	0x06
#define BYTE4_ADDRESS_ENABLE_CMD 	0xB7
#define QPI_MODE_ENABLE_CMD  		0x35
#define SECTOR_ERASE_4BYTE_CMD  	0x21
#define RD_XTD_RDREG_CMD  			0x81
#define CLR_XTD_RDREG_CMD  			0x82
#define PAGE_PROGRAM_4PP_CMD		0x12
#define FAST_READ_4FRD_CMD			0x0C
/*#define WR_DISABLE_LATCH_SPI_CMD			0x04

 #define BYTE_PAGE_PGM_SPI_CMD  				0x02
 #define RD_DATA_BYTES_SPI_CMD  				0x03
 #define RD_STATUS_REG_SPI_CMD				0x05
 #define RDID_SPI_CMD					0x83
 /***************************************************/



#define FLASH_MEMORY_SIZE	0X19
typedef unsigned char UC; //1 Byte
typedef unsigned short US; //2 Bytes
typedef unsigned int UI; //4 Bytes
typedef unsigned long UL; //8 Bytes

typedef struct {
	UL QSPI_CSR;
	UL QSPI_DCR;
	UL QSPI_ADLR;
	UL QSPI_CCR;
	UL QSPI_DR;
	UL QSPI_SPR;
	UL QSPI_FCR;
	UL QSPI_DSR;

} QSPI_REG_TYPE;
#define QSPI_0 0
#define QSPI_1 1
typedef union {
	struct {
		UC CSR_LSB; //Enable,Abort request,FTh
		UC CSR_CsIE; //Chip select ,interrupt enable,AMPS and PMM
		US CSR_Prescalar;
		UC CSR_Flag; //Flags
		UC CSR_FLevel;
		US dummy;
	} Field;

	UL value;
} CSR_CONFG_TYPE;

typedef union {
	struct {
		US DCR_ModeCHST :4, //level that Sclk takes between commands and Chip Select High Time
				DCR_FSize :5, //Flash memory size
				dummy1 :7;
		US dummy2;
		UI dummy3;
	} Field;

	UL value;
} DCR_CONFG_TYPE;

//Register address mapping
#define QSPIReg(i) (*((volatile QSPI_REG_TYPE *)(QSPI_BASE_ADDR+ (0x100 * i))))



/*  Function declaration section
 *
 *
 ***************************************************/

void qspi_initialise(UC qspi_number);
void qspi_read_flash_register(UC qspi_number, UL address, UI dataLength,
		UC command, UC instr_line, UC data_line, UC *read_data);

void qspi_instrTxt(UC qspi_number, UC instr_line, UC command);

void qspi_write_flash_register(UC qspi_number, UL address, UI dataLength,
		UC command, UC instr_line, UC data_line, UC wr_data);

void qspi_mem_write(UC qspi_number, UL Address, UI DataLength, UC command,
		UC instr_line, UC data_line, UC data_size, UC address_line,
		UC address_size, UC fifo_threshold, UC *write_data);

void qspi_mem_read(UC qspi_number, UL Address, UI DataLength, UC command,
		UC instr_line, UC data_line, UC data_size, UC address_line,
		UC address_size, UC fifo_threshold, UC *read_data);

UC qspi_wait_if_busy(UC qspi_number);

void qspi_mem_erase(UC qspi_number, UL Address, UI DataLength, UC command,
		UC instr_line, UC address_line, UC address_size) ;

void qspi_write_read_444P_adrsize3(UC qspi_number, UI Pages, UI address,
		UC Wfifo_thres, UC WData_size, UC Rfifo_thres, UC RData_size);
void qspi_flash_read(UC qspi_number, UI RAddress, UI Rdata_len, UC i_line,
		UC d_line, UC d_size, UC addr_line, UC addr_size, UC fifo_thres,UC *read_data);
void qspi_flash_write(UC qspi_number, UI WAddress, UI Wdata_len, UC i_line,
		UC d_line, UC d_size, UC addr_line, UC addr_size, UC fifo_thres,UC *wr_data);
void qspi_flash_serase(UC qspi_number, UI SAddress, UI SData_len, UC i_line,
		UC d_line, UC addr_line, UC addr_size);
void qspi_init_transfer(UC qspi_number, UI Clk_Prescalar, UC fifo_thres,
		UC Auto_poll_stop, UC Poll_Match);
void flash_confg_444(UC qspi_number);
void flash_busy_check(UC qspi_number, UC i_line, UC d_line);

UC comparedata(UC *bWrData, UC *bRdData, UL wDataLength);
void flash_read_id(UC qspi_number);
void board_setup(UC qspi_number);
void qspi_delay(UL count);


#endif

#endif

