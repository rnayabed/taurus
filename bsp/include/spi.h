#ifndef _SPI_H
#define _SPI_H
/**
 @file spi.h
 @brief header file for SPI driver
 @detail 
 */
/***************************************************
 * Module name: spi.h
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
 * SPI registers and function declarations
 *
 ***************************************************/

/*  Include section
 *
 ***************************************************/
#include "config.h"
/*  Define section
 *
 *
 ***************************************************/
#define MDP_SPI_0			0
#define MDP_SPI_1			1
#define MDP_SPI_2			2
#define MDP_SPI_3			3

#define SPI_INTR_MODE			1
#define SPI_POLLING_MODE		2
#define DATA_BYTES 			65536

// SPI BASE ADDRESS

#define LOW				0
#define HIGH 				1

#define MSB				0
#define LSB 				1

#define SPI_FIXD_PERIPH         	0
#define SPI_VAR_PERIPH          	1 

#define CPOL_MODE_0         		0
#define CPHA_MODE_0         		0  
#define CPOL_MODE_3         		1
#define CPHA_MODE_3         		1 

#define BPT_8				0

#define TXINTR_DIS			0
#define RXINTR_DIS			0

#define TXINTR_EN			1
#define RXINTR_EN			1

#define SPI_MODE_0			0
#define SPI_MODE_3			3

#define SPI_CS_0			0
#define SPI_CS_1			1
#define SPI_CS_2			2
#define SPI_CS_3			3

#define RX_TX_INTR_DIS			0
#define TX_DIS_RX_EN_INTR		1
#define RX_DIS_TX_EN_INTR		2
#define RX_TX_INTR_EN			3

#define SPI_BAUD_CFD_4        		0
#define SPI_BAUD_CFD_8        		1 
#define SPI_BAUD_CFD_16        		2 
#define SPI_BAUD_CFD_32        		3
#define SPI_BAUD_CFD_64        		4
#define SPI_BAUD_CFD_128        	5 
#define SPI_BAUD_CFD_256       		6
#define SPI_BAUD_CFD_512        	7 
#define SPI_BAUD_CFD_1024        	8 
#define SPI_BAUD_CFD_2048        	9

#define SPI_BUSY_BIT            	(1<<4)
#define SPI_OVERR_BIT           	(1<<5)
#define SPI_RX_COMPLETE_BIT    		(1<<6)
#define SPI_TX_HOLD_EMPTY_BIT   	(1<<7) 

#define SPI_RX_INT_STATUS_BIT       	(1<<2)
#define SPI_TX_INT_STATUS_BIT       	(1<<3)

#define SPI_RX_INTR			1
#define SPI_TX_INTR			2

/*************Micron M25P80 Serial Flash************* 
 *************Embedded Memory Command Set************/
/*#define WR_DISABLE_LATCH_SPI_CMD			0x04
 #define WR_EN_LATCH_SPI_CMD     			0x06
 #define BYTE_PAGE_PGM_SPI_CMD  				0x02
 #define RD_DATA_BYTES_SPI_CMD  				0x03
 #define RD_STATUS_REG_SPI_CMD				0x05
 #define RDID_SPI_CMD					0x83
 /***************************************************/

typedef struct {

	union {
		struct {
			US PeriphCS :2, Periph :1, DataOrder :1, Mode :2, SPIrxTxIntr :2,
					CSAAT :1, Dbits :4, Reserved :3;
		}__attribute__((packed)) Bits;
		US hword;
	} Control; 					//0x00
	UI Status; 					//0x04
	UI Baudrate;					//0x08
	UI TxData;					//0x0c
	UI RxData;					//0x10
} SPIregType;

typedef union {
	struct {
		US PeriphChipSelect :2, PeriphType :1, DataOrder :1, Mode :2, SPIrxTxIntr :2,
				CSAAT :1, Dbits :4, Reserved :3;
	} Bits;
	US Value;
} SPIcntrlRegType;

//Register address mapping
#define SPIreg(i) (*((volatile SPIregType *)(SPI_BASE_ADDR(i) + (0x100 * (i % 2)))))
#define SPI_VARIABLE_PERIPHERAL	1
#define SPI_FIXED_PERIPHERAL	0
#define SPI_MSB_FIRST			0
#define SPI_LSB_FIRST			1

/*  Function declaration section
 *
 *
 ***************************************************/

void SPI_init(UC spi_number);
void SPI_config(UC spi_number, SPIcntrlRegType spi_control_reg);
void SPI_set_baud(UC spi_number, UC bBaudCFD);
UI SPI_get_slave_select(UC spi_number);
void SPI_set_slave_select(UC spi_number, UI slave_sel_val);
UC SPI_check_overrun(UC spi_number);
void SPI_set_intr(UC spi_number, UC tx_intr, UC rx_intr);
US SPI_receive(UC spi_number);
void SPI_transmit(UC spi_number, US bData);
void SPI_intr_handler(UC spi_number);
void SPI_wait_if_busy(UC spi_number);
void SPI_set_CSAAT_pin(UC spi_number, UC status);
US SPI_read_rx_reg(UC spi_number);
void SPI_write_tx_reg(UC spi_number, US bData);
UC spi_intr_status(UC spi_number);
void spi_register_isr(UC spi_number, void (*spi_isr)());

//void (*spi_handle_rx_intr)(void);  
//void (*spi_handle_tx_intr)(void);

#endif	/* _SPI_H */	

