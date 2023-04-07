/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Nov-2019
 Filename		: debug_uart.c
 Purpose		: Common interface for uart
 Description		: UART tx/rx functions
 Author(s)		: Premjith A V, Sreenadh S
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/

/**
 @file debug_uart.c
 @brief Contains routines for basic uart for debugging/Printf 
 @detail Includes software functions to initialize,
 configure, transmit and receive over UART
 */

#include <include/debug_uart.h>

/**************************************************
 * Function name	: void init_uart(void)
 * returns		    : Nil
 * Created by		: Sreenadh.S
 * Date created		: 04/01/2007
 * Description		: Initialize UART.
 * Notes			: To initialize UART - Baud Rate = 115200 Clock 25MHz
 *                     	8 Data bits, 1 Stop bit, no Parity,disable DR intrpt &
 *                     	THRE intrpt
 **************************************************/
/**
@fn init_uart
@brief default baud rate and frame initialization
@details To initialize UART: Baud Rate = 115200 Clock 25MHz
Calculate Divisor(Divisor = Input frequency / (Baud rate X 16) )
for the baud rate  and configure uart register. UART frame is initialized by
setting the data bits,parity bits and stop bits
8 Data bits, 1 Stop bit, no Parity,
Disable DR interrupt & THRE interrupt
@param[Out] No ouput parameter.
@return Void function.
*/
void init_uart(void) {
	uart_regs.UART_LCR = 0x83; //Divisor latch enabled
	uart_regs.UART_DR = 0x15; //0x0b-20mHz,0x02-5mHz,0x10-30mHz;//0x0e-25mHz;0x0f-29mHz,0x1b-50mHz; 0x15-40mHz ;0x28-75mHz  //Divisor LSB
	uart_regs.UART_IE = 0x00; //Divisor MSB
	uart_regs.UART_LCR = 0x03; //Divisor latch disabled, stop-bits = 1, parity = none, data-bits = 8
	uart_regs.UART_IE = 0x00; //Interrupts disabled
	__asm__ __volatile__ ("fence");
}

/**************************************************
 * Function name	: void tx_uart(UC tx_char)
 *    returns		: Nil
 * Tx_char           : Character to Tx
 * Created by		: Sreenadh.S
 * Date created		: 04/01/2007
 * Description		: Tx 1 character through UART
 * Notes			:
 *************************************************/
/**
 @fn void void tx_uart(UC tx_char)
 @brief 1 byte character transmission
 @details Transmit 1 character through uart.Proceeds only when transmitter is idle
 @param[in] unsigned char(bTxCharacter--character to be transmitted)
 @return Void function.

 */
void tx_uart(UC tx_char) {
	UC lsr;

	uart_regs.UART_DR = tx_char;
	__asm__ __volatile__ ("fence");
	do {
		lsr = uart_regs.UART_LSR;
		__asm__ __volatile__ ("fence");
		lsr = lsr & 0x20;
	} while (lsr != 0x20);
}

/**************************************************
 * Function name	: UC Tx_uart(void)
 *    returns		: Nil
 * Tx_char           : Character to Rx
 * Created by		: Sreenadh.S
 * Date created		: 04/01/2007
 * Description		: Rx 1 character through UART
 * Notes			:
 **************************************************/
/**
 @fn UC rx_uart(void)
 @brief 1 byte character reception
 @details Receives 1 character thcrough uart
 @return unsigned char Rxd_data--data received

 */
UC rx_uart(void) {
	UC lsr, Rx_char;

	do {
		lsr = uart_regs.UART_LSR;
		__asm__ __volatile__ ("fence");
	} while ((lsr & 1) == 0);

	Rx_char = uart_regs.UART_DR;
	__asm__ __volatile__ ("fence");

	//uart_regs.UART_LSR &= ~0x1;  //For emulation only FIXME
	return Rx_char;
}

/**************************************************
 * Function name	: get_decimal()
 * returns		    : Nil
 * Created by		: Sreeju.GR
 * Date created		: 22/11/2018
 * Description		:
 * Notes			:
 **************************************************/
/**
 @fn UL get_decimal(UC noOfDigits) 
 @brief get decimal value of entered ascii digits
 @details Receives number of digits to read
 @return unsigned long Returns the decimal value

 */
UL get_decimal(UC noOfDigits) {
	UC i, rx = 0, ascii[16];
	;
	UL number = 0, cnt = 0, temp = 1;

	for (i = 1; i < noOfDigits; i++)
		temp *= 10;

	i = 0;
	while (1) {

		if (i < noOfDigits) {
			while (1) {
				rx = rx_uart();
				if (rx >= '0' && rx <= '9') {
					tx_uart(rx);
					ascii[i] = rx;
					break;
				}
				if (rx == '\b') {
					if (i > 0) {
						tx_uart(rx);
						tx_uart(' ');
						tx_uart(rx);
						i--;
					}
					continue;
				}

			}
			i++;

		} else if (i == noOfDigits) {
			rx = rx_uart();
			if (rx == '\r') {
				break;
			} else if (rx == '\b') {
				tx_uart(rx);
				tx_uart(' ');
				tx_uart(rx);
				i--;
				continue;
			}
		}
	}
	for (i = 0; i < noOfDigits; i++) {
		ascii[i] -= '0'; //ascii
		number = number + (temp * ascii[i]);
		temp = temp / 10;
	}
	return number;
}

/**************************************************
 * Function name	: get_long_int()
 * returns		    : Nil
 * Created by		: Sreeju.GR
 * Date created		: 22/11/2018
 * Description		:
 * Notes			:
 **************************************************/
/**
 @fn UL get_long_int(UC noofBytes)
 @brief get long integer value of entered ascii digits
 @details Receives number of digits to read
 @return unsigned long Returns the long integer value

 */
UL get_long_int(UC noofBytes) {
	UC i, rx = 0, temp[16];
	UI hex_val = 0;
	i = 0;
	while (1) {
		if (i < noofBytes) {
			/*if ((noofBytes == 4) || (noofBytes == 2)) {
			 if (i > 0)
			 Tx_uart('-');
			 }*/
			rx = get_hex();
			if (rx == '\b') {
				if (i > 0) {
					tx_uart(rx);
					tx_uart(' ');
					tx_uart(rx);
					tx_uart(rx);
					tx_uart(' ');
					tx_uart(rx);
					i--;
				}
				continue;
			}
			//temp <<= 8;
			//temp = temp + rx;
			temp[i] = rx;

			i++;
		} else if (i == noofBytes) {
			rx = rx_uart();
			if (rx == '\r') {
				break;
			} else if (rx == '\b') {
				tx_uart(rx);
				tx_uart(' ');
				tx_uart(rx);
				tx_uart(rx);
				tx_uart(' ');
				tx_uart(rx);
				i--;
				continue;
			}
		}
	}

	for (i = 0; i < noofBytes; i++) {
		hex_val <<= 8;
		hex_val |= temp[i];
	}

	return hex_val;
}

/************************************************************
 * Function name		: UC get_hex()
 * returns		    : 1 byte unsigned character (number).
 * Date created		: 22/07/2009
 * Description		: To get hex value and display it to HT.
 *************************************************************/
/**
 @fn UC get_hex()
 @brief get hexadecimal value of entered digits
 @details Receives number of digits to read
 @return unsigned long Returns the hexadecimal value

 */
UC get_hex() {
	unsigned char number = 0, dig1, dig2, count = 2, rx;
	while (count) {
		rx = rx_uart();
		if (rx == '\b') {
			if (count < 2) {
				tx_uart(rx);
				tx_uart(' ');
				tx_uart(rx);
				count++;
				continue;
			} else
				return rx;
		}

		if ((rx >= 0x30) && (rx <= 0x39)) {
			tx_uart(rx);
			rx = rx - 0x30;
		} else if ((rx >= 'A') && (rx <= 'F')) {
			tx_uart(rx);
			rx = rx - 0x37;
		} else if ((rx >= 'a') && (rx <= 'f')) {
			tx_uart(rx);
			rx = rx - 0x57;
		}

		else
			continue;
		if (count == 2)
			dig1 = rx;
		else
			dig2 = rx;
		count--;
	}
	number = (dig1 << 4);
	number |= dig2;
	return number;
}

