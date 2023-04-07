/***************************************************
 * Module name: main.c
 *
 * Copyright 2020 Company CDAC(T)
 * All Rights Reserved.
 *
 *  The information contained herein is confidential
 * property of Company. The user, copying, transfer or
 * disclosure of such information is prohibited except
 * by express written agreement with Company.
 *
 *
 * Module Description:
 * QSPI sample pgm
 *
 ***************************************************/

/*  Include section
 *
 *
 ***************************************************/
/**
 @file qspi_demo.c
 @brief Contains routines for QSPI Flash read/write
 */
/** 
 @brief writes and reads flash with QSPI interface
 @details Initializes configures QSPI to erase write, read and compare data in FLASH
 @param[in] No input parameters.
 @param[Out] No ouput parameter.
 @return Void function.

 */


#include "stdlib.h"
#if __riscv_xlen == 64 //QSPI driver available only for thejas64
#include "qspi.h"
#include "debug_uart.h"

void main() {

	board_setup(QSPI_0);

	//flash_read_id(QSPI_0);

	qspi_write_read_444P_adrsize3(QSPI_0, 01, 0X0000, 63,
	DATA_SIZE_3, 63, DATA_SIZE_3);

	while (1)
		;
}

/**
 @fn flash_read_id
 @brief reads Flash identification register
 @details  Read access to manufacturer identification, device identification, and Common
 Flash Interface (CFI) information and QRY
 @param[in] unsigned char (qspi_number)
 @param[Out] No ouput parameter.
 @return Void function.

 */

void flash_read_id(UC qspi_number) {

	UC flash_id[3];

	qspi_read_flash_register(qspi_number, 0x00, 0x03, RD_FLASH_ID_CMD,
	INSTR_LINE_1, DATA_LINE_1, flash_id);

	for (int i = 0; i < 3; i++) {
	//	printf("\n\r flash_id :%x", flash_id[i]);

	//	printf("\n\r");
	}

}

/**
 @fn board_setup
 @brief Initialises QSPI AND Resets the board
 @details
 @param[in] unsigned char (qspi_number)
 @param[Out] No ouput parameter.
 @return Void function.

 */

void board_setup(UC qspi_number) {



	qspi_delay(10000);

	qspi_initialise(qspi_number);

	QSPIReg(qspi_number).QSPI_ADLR = 0X00;

	qspi_instrTxt(qspi_number, INSTR_LINE_3, RESET_ENABLE_CMD);

	qspi_instrTxt(qspi_number, INSTR_LINE_3, RESET_FLASH_CMD);

	qspi_delay(10000);



}

/**
 @fn qspi_delay
 @brief Provides required amount of delay
 @details
 @param[in] unsigned long (count)
 @param[Out] No ouput parameter.
 @return Void function.

 */

void qspi_delay(UL count) {

	for (UL d = 1; d <= count; d++) //delay 25MHz
			{
	}

}

/**************************************************
 * Function name	: TxHexUartByteDbg
 * Created by	: Sreenadh.S
 * Date created	: 04/08/2007
 * Description	: Display data in Hexadecimal
 * Notes			:
 **************************************************/
/*
 void TxHexUartByteDbg(UC bDisplayData) {

 UC bTemp = 0;
 UC bAscii = 0;

 bTemp = bDisplayData;

 bTemp = (bTemp >> 4) & 0x0f;
 if ((bTemp > 9)) {
 bTemp = bTemp - 9;
 bAscii = bTemp + 0x40;
 } else {
 bAscii = bTemp + 0x30;
 }
 tx_uart(bAscii);

 bTemp = bDisplayData;
 bTemp = bTemp & 0x0f;
 if ((bTemp > 9)) {
 bTemp = bTemp - 9;
 bAscii = bTemp + 0x40;
 } else {
 bAscii = bTemp + 0x30;
 }
 tx_uart(bAscii);

 }
 */
/**
 @fn flash_busy_check
 @brief check whether flash is busy
 @details checks WIP of flash status register
 @param[in] unsigned char (qspi_number)
 @param[Out] No ouput parameter.
 @return Void function.

 */

void flash_busy_check(UC qspi_number, UC i_line, UC d_line) {

	//printf("\n\r flashbusy");
	UC flash_status;
	while (1) {
		//printf("\n\r flash while");
		qspi_read_flash_register(qspi_number, 0x00, 0x01,
		RD_STATUS1_REG_QSPI_CMD, i_line, d_line, &flash_status);

		if ((flash_status & 0x01) == 0x00) {	//checks if flash is idle
			break;
		}

		QSPIReg(qspi_number).QSPI_FCR = 0x4F;	//cleared all flags except busy
	}

	QSPIReg(qspi_number).QSPI_FCR = 0x4F;	//cleared all flags except busy

	__asm__ __volatile__ ("fence");

}
/**
 @fn flash_confg_444
 @brief Configuring Configuration registers of flash in 444P
 @details Write enable,set to 4 byte address mode and QPI mode
 @param[in] unsigned char (qspi_number)
 @param[Out] No ouput parameter.
 @return Void function.
 */

void flash_confg_444(UC qspi_number) {

	printf("\n\r configuration start...");

	flash_busy_check(qspi_number, INSTR_LINE_1, DATA_LINE_1);//flash busy check with 111Protocol
	qspi_instrTxt(qspi_number, INSTR_LINE_1, WR_EN_LATCH_FLASH_CMD);

	flash_busy_check(qspi_number, INSTR_LINE_1, DATA_LINE_1);//flash busy check with 111Protocol

	qspi_instrTxt(qspi_number, INSTR_LINE_1, BYTE4_ADDRESS_ENABLE_CMD);

	flash_busy_check(qspi_number, INSTR_LINE_1, DATA_LINE_1);//flash busy check with 111Protocol

	qspi_instrTxt(qspi_number, INSTR_LINE_1, QPI_MODE_ENABLE_CMD);

	flash_busy_check(qspi_number, INSTR_LINE_3, DATA_LINE_3);//flash busy check with 444Protocol
	printf("\n\r configuration complete");

}

/**
 @fn flash_memory_serase
 @brief sector erase
 @details  Checks whether Qspi is busy
 Checks whether flash is busy
 WREN (cmd:0x06)in flash is enabled(Configured CCR and checked transfer complete)
 Set Address and data length
 sector erase command(Configured CCR and checked transfer complete)
 Check for completion of erase
 Check for sector erase error
 @param[in] unsigned char (qspi_number)
 @param[in] unsigned long (address)
 @param[in] unsigned int (dataLength)
 @param[in] unsigned char (instr_line)
 @param[in] unsigned char (data_line)
 @param[in] unsigned char (address_line)
 @param[in] unsigned char (addr_size)
 @param[Out] No ouput parameter.
 @return Void function.

 */

void qspi_flash_serase(UC qspi_number, UI SAddress, UI SData_len, UC i_line,
		UC d_line, UC addr_line, UC addr_size) {

	UC error_status;

	qspi_instrTxt(qspi_number, i_line, WR_EN_LATCH_FLASH_CMD);

	while (1) {

		flash_busy_check(qspi_number, i_line, d_line); //flash busy check with 444Protocol
		printf("\n\r mem erase s1");
		qspi_mem_erase(qspi_number, SAddress, SData_len,
		SECTOR_ERASE_4BYTE_CMD, i_line, addr_line, addr_size);
		printf("\n\r flashbusy s2");
		flash_busy_check(qspi_number, i_line, d_line); //flash busy check with 444Protocol
		printf("\n\r CSR set");
		qspi_init_transfer(qspi_number, PRESCALAR_MAX, 00, AUTO_POLL_STOP_OFF,
		POLL_MODE_AND);
		printf("\n\r serror fn");
		qspi_read_flash_register(qspi_number, 0x00, 0x01, RD_XTD_RDREG_CMD,
				i_line, d_line, &error_status);

		if ((error_status & 0x08) == 0x08) {
			qspi_instrTxt(qspi_number, i_line, CLR_XTD_RDREG_CMD);

			continue;
		} else {

			break;
		}
	}
	printf("\n\r serase fn end");
}

/**
 @fn qspi_flash_write
 @brief flash page program
 @details  Checks whether Qspi is busy
 Checks whether flash is busy
 WREN (cmd:0x06)in flash is enabled(Configured CCR and checked transfer complete)
 Configured the QSPI control(prescalar,CS0,FTH set and QSPI enabled)
 Set address and datalength for data
 Flash Page Programming
 write data to Fifo upto threshold after checking Fifo is empty (after setting DSR ,write data to DR)
 when reaches the FTH, check end transfer and flash busy check
 @param[in] unsigned char (qspi_number)
 @param[in] unsigned long (Waddress)
 @param[in] unsigned int (Wdata_len)
 @param[in] unsigned char (i_line)
 @param[in] unsigned char (d_line)
 @param[in] unsigned char (address_line)
 @param[in] unsigned char (addr_size)
 @param[in] unsigned char (fifo_thres)
 @param[Out] No ouput parameter.
 @return Void function.
 */

void qspi_flash_write(UC qspi_number, UI WAddress, UI Wdata_len, UC i_line,
		UC d_line, UC d_size, UC addr_line, UC addr_size, UC fifo_thres,
		UC *wr_data) {

	UC error_status;

	while (1) { //checks whether busy

		if (qspi_wait_if_busy(qspi_number))
			continue;
		else
			break;
	}
	while (1) {
		flash_busy_check(qspi_number, i_line, d_line); //flash busy check with 444Protocol
		qspi_instrTxt(qspi_number, i_line, WR_EN_LATCH_FLASH_CMD);
		flash_busy_check(qspi_number, i_line, d_line); //flash busy check with 444Protocol
		qspi_init_transfer(qspi_number, PRESCALAR_MAX, (fifo_thres - 1),
		AUTO_POLL_STOP_OFF,
		POLL_MODE_AND);
		qspi_mem_write(qspi_number, WAddress, Wdata_len, PAGE_PROGRAM_4PP_CMD,
				i_line, d_line, d_size, addr_line, addr_size, fifo_thres,
				wr_data);

		flash_busy_check(qspi_number, i_line, d_line); //flash busy check with 444Protocol

		qspi_read_flash_register(qspi_number, 0x00, 0x01, RD_XTD_RDREG_CMD,
				i_line, d_line, &error_status);

		if ((error_status & 0x04) == 0x04) {
			qspi_instrTxt(qspi_number, i_line, CLR_XTD_RDREG_CMD);

			continue;
		} else {

			break;
		}

	}
}
/**
 @fn qspi_flash_read
 @brief flash page read
 @details  Checks whether Qspi is busy
 Checks whether flash is busy
 WREN (cmd:0x06)in flash is enabled(Configured CCR and checked transfer complete)
 Configured the QSPI control(prescalar,CS0,FTH set and QSPI enabled)
 Set address and datalength for data
 Flash read Programming
 read data from Fifo(after setting DSR ,write data to DR)
 check end read transfer
 @param[in] unsigned char (qspi_number)
 @param[in] unsigned long (Waddress)
 @param[in] unsigned int (Wdata_len)
 @param[in] unsigned char (i_line)
 @param[in] unsigned char (d_line)
 @param[in] unsigned char (address_line)
 @param[in] unsigned char (addr_size)
 @param[in] unsigned char (fifo_thres)
 @param[Out] No ouput parameter.
 @return Void function.
 */
void qspi_flash_read(UC qspi_number, UI RAddress, UI Rdata_len, UC i_line,
		UC d_line, UC d_size, UC addr_line, UC addr_size, UC fifo_thres,
		UC *read_data) {
	printf("\n\r read");
	while (1) { //checks whether busy

		if (qspi_wait_if_busy(qspi_number))
			continue;
		else
			break;
	}
	flash_busy_check(qspi_number, i_line, d_line); //flash busy check with 444Protocol
	qspi_instrTxt(qspi_number, i_line, WR_EN_LATCH_FLASH_CMD);
	flash_busy_check(qspi_number, i_line, d_line); //flash busy check with 444Protocol
	qspi_init_transfer(qspi_number, PRESCALAR_MAX, (fifo_thres - 1),
	AUTO_POLL_STOP_OFF,
	POLL_MODE_AND);
	qspi_mem_read(qspi_number, RAddress, Rdata_len, FAST_READ_4FRD_CMD, i_line,
			d_line, d_size, addr_line, addr_size, fifo_thres, read_data);

}
/**
 @fn qspi_write_read_444P_adrsize3
 @brief erase,write and read using 444Protocol and 32 bit addressing
 @details  444 Protocol-instruction,address and data in quad lines
 @param[in] unsigned char (qspi_number)
 @param[in] unsigned int (Pages)
 @param[in] unsigned int (address)
 @param[in] unsigned char (Wfifo_thres)
 @param[in] unsigned char (WData_size)
 @param[in] unsigned char (address_line)
 @param[in] unsigned char (Rfifo_thres)
 @param[in] unsigned char (RData_size)
 @param[Out] No ouput parameter.
 @return Void function.

 */
void qspi_write_read_444P_adrsize3(UC qspi_number, UI Pages, UI address,
		UC Wfifo_thres, UC WData_size, UC Rfifo_thres, UC RData_size) {
	//printf("\n\r write read erase in 444P");
	UC data_arr[1024];
	UC rxd_data[1024];
	UC bStatus;
	UL rand_value = 99;

	for (UL i = 0; i < (Pages * 256); i++) {

		rand_value = rand_value * 5;
		if (rand_value == 0)
			rand_value = 99;
		data_arr[i] = (UC) rand_value;

	}

	flash_confg_444(qspi_number);
	qspi_init_transfer(qspi_number, PRESCALAR_MAX, 00,
	AUTO_POLL_STOP_OFF,
	POLL_MODE_AND);
	UI erased_sector = 0xFFFFFFFF;
	//gptr = 0;
	printf("\n\rerase in 444P");
	for (int i = 0; i < Pages; i++) {
		UI current_page_addr = address + (256 * i);
		UI current_sector = current_page_addr & 0xFFFFF000;

		if (erased_sector != current_sector) {
			qspi_flash_serase(qspi_number, current_sector, 00, INSTR_LINE_3,
			DATA_LINE_3, ADDR_LINE_3, ADDR_SIZE_3);
			erased_sector = current_sector;

		}

		qspi_init_transfer(qspi_number, PRESCALAR_MAX, Wfifo_thres,
		AUTO_POLL_STOP_OFF,
		POLL_MODE_AND);
		qspi_flash_write(qspi_number, current_page_addr, 0x100, INSTR_LINE_3,
		DATA_LINE_3, WData_size, ADDR_LINE_3, ADDR_SIZE_3, (Wfifo_thres + 1),
				data_arr);
		//Datalength 256 byte

	}
printf("\n\r CSR for read");
	qspi_init_transfer(qspi_number, PRESCALAR_MAX, Rfifo_thres,
	AUTO_POLL_STOP_OFF,
	POLL_MODE_AND);
	qspi_flash_read(qspi_number, address, (Pages * 256), INSTR_LINE_3,
	DATA_LINE_3, RData_size, ADDR_LINE_3, ADDR_SIZE_3, (Rfifo_thres + 1),
			rxd_data); //Datalength 256 byte

	bStatus = comparedata(data_arr, rxd_data, (Pages * 256));

	if (bStatus) //Wrong Data
		printf("\n\r COMPARISON -  FAILED\n\r");
	else
		//Right Data
		printf("\n\r COMPARISON -  SUCCESS\n\r");

}

/** @fn comparedata
 @brief Compare written value with read value from flash.
 @details Compare the original data with the received data from device.
 @warning
 @param[in] unsigned char *, unsigned char *, unsigned long
 @param[Out] status = 1 when comparison fails, status = 0 when comparison passes.
 */
UC comparedata(UC *bWrData, UC *bRdData, UL wDataLength) {
	UC status = 0;
	UI i;
	for (i = 0; i < wDataLength; i++) {
		if (*bWrData == *bRdData)
			status = 0;
		else {
			status = 1;
			return status;
		}
		bWrData++;
		bRdData++;
	}
	return status;
}

#else
void main() {
printf("QSPI is only available in THEJAS64 SOC\n");
	
	while (1)
		;
}
#endif
