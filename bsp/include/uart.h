/**
 @file uart.h
 @brief header file for UART driver
 @detail 
 */

#ifndef __UART_H
#define __UART_H

typedef unsigned char UC; 		//1 Byte
typedef unsigned short US; 		//2 Bytes
typedef unsigned int UI; 		//4 Bytes
typedef unsigned long UL; 		//8 Bytes

typedef struct
{
	UI UART_DR;
	UI UART_IE;
	UI UART_IIR_FCR;
	UI UART_LCR;
	UI Dummy;
	UI UART_LSR;
} UART_REG_TYPE;

#define UART_0					0
#define UART_1					1
#define UART_2					2

#define UART_NO_ERROR 			0
#define UART_PARITY_ERROR 		-1
#define UART_OVERRUN_ERROR 		-2
#define UART_FRAMING_ERROR 		-3

#define TX_ENABLE				1
#define TX_DISABLE				0
#define RX_ENABLE				1
#define RX_DISABLE				0

#define UartReg(i) (*((volatile UART_REG_TYPE *)(UART_BASE+ (0x100 * i))))

void uart_init(UC uart_number);
void uart_configure(UC uart_number, UL Baud_rate, UL frame_value, UL Uart_clock);
void uart_putchar(UC uart_number, UC bTxCharacter, char *error);
UC uart_getchar(UC uart_number, char *error);
UC uart_getchar_async(UC uart_number, char *error);
void uart_intr_enable(UC uart_number, UC tx_intr, UC rx_intr);
void uart_set_baud_rate(UC uart_number,UL Baud_rate, UL Uart_clock);
void uart_intr_enable(UC uart_number, UC tx_intr, UC rx_intr);
void uart_tx_rx(UC TxUart,UC RxUart);
UC uart_getchar_sim_txn_rxn(UC uart_number_tx,UC uart_number_rx, char *error,char txdata);

#endif /*__UART_H*/