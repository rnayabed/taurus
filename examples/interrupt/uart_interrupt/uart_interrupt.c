/***************************************************************************
* Module name                           :  uart_interrupt.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  uart working on interrupt mode.

  Copyright (C) 2020  CDAC(T). All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/

#include "uart.h"
#include "stdlib.h"
#include "config.h"
#include "interrupt.h"

#if __riscv_xlen == 64
#define UART0_INTR_NUM 1 // Note : this number is for thejas64, number may change on other SoCs
#else
#define UART0_INTR_NUM 0 // Note : this number is for thejas32, number may change on other SoCs
#endif
void uart_intr_handler(void) // interrupt handler
{
	unsigned char status, error, data;

	status = UartReg(UART_0).UART_IIR_FCR; // clearing interrupt
	data = uart_getchar(UART_0, &error);   // reading data
	uart_putchar(UART_0, data, &error);	   // echoing data
	return;
}

/**
 @fn main
 @brief transmit and reception through uart
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 */
void main()
{

	uart_set_baud_rate(UART_0, 115200, 40000000); // initializing uart0
	printf("+------[ uart%d in interrupt mode ]------+\n", UART_0);

	interrupt_enable(UART0_INTR_NUM); //Enable interrupt in controller.
	irq_register_handler(UART0_INTR_NUM, uart_intr_handler);
	uart_intr_enable(UART_0, TX_DISABLE, RX_ENABLE); // enabling uart0 interrupt
	printf("\nStart typing anything on uart%d.\n\n", UART_0);
		/* idle loop */
		while (1);
}
