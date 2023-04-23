/**
 @file uart.c
 @brief Contains routines for UART interface
 @detail Includes software functions to initialize,
 configure, transmit and receive over UART
 */

#include <stdlib.h>
#include <config.h>
#include <uart.h>

/**
 @fn void uart_init(UC uart_number)
 @brief default baud rate and frame initialization
 @details To initialize UART: Baud Rate = 115200 Clock 25MHz
 Calculate Divisor(Divisor = Input frequency / (Baud rate X 16) )
 for the baud rate  and configure uart register. UART frame is initialized by
 setting the data bits,parity bits and stop bits
 8 Data bits, 1 Stop bit, no Parity,
 Disable DR interrupt & THRE interrupt
 @param[in] unsigned char(uart_number)
 @param[Out] No ouput parameter.
 @return Void function.
*/
void uart_init(UC uart_number) {
	UartReg(uart_number).UART_LCR = 0x83;
	UartReg(uart_number).UART_DR = 0x0e; //LSB
	UartReg(uart_number).UART_IE = 0x00; //MSB
	UartReg(uart_number).UART_LCR = 0x03;
	UartReg(uart_number).UART_IE = 0x00;
}


/**
 @fn void uart_set_baud_rate(UC uart_number,UL Baud_rate, UL Uart_clock)
 @brief set baud rate for uart
 @details Calculate Divisor(Divisor = Input frequency / (Baud rate X 16) )
 for the baud rate  and configure uart register. UART frame is initialized by
 setting the data bits,parity bits and stop bits
 8 Data bits, 1 Stop bit, no Parity,
 Disable DR interrupt & THRE interrupt
 @param[in] unsigned char(uart_number)
 @param[in] unsigned long(Baud_rate)
 @param[in] unsigned long(Uart_clock)
 @param[Out] No ouput parameter.
 @return Void function.
*/
void uart_set_baud_rate(UC uart_number,UL Baud_rate, UL Uart_clock){
	UL divisor;
		divisor = (Uart_clock / (Baud_rate * 16));
		UartReg(uart_number).UART_LCR = 0x83;
		UartReg(uart_number).UART_DR = divisor & 0xFF; //LSB
		UartReg(uart_number).UART_IE = (divisor >> 0x08) & 0xFF; //MSB(right shift)
		UartReg(uart_number).UART_LCR = 0x03;
		UartReg(uart_number).UART_IE = 0x00;
}


/**
 @fn void uart_configure(UC uart_number, UL Baud_rate, UL frame_value, UL Uart_clock)
 @brief Baud rate and Frame initialization
 @details Calculate Divisor(Divisor = Input frequency / (Baud rate X 16) )
 for the baud rate  and configure uart register. UART frame is initialized by
 setting the data bits,parity bits and stop bits
 Frame value is set as follows:
 Bit 7: Divisor latch access .1 for setting baud rate and 0 for access rxr buffer
 Bit 6: Break control bit
 Bit 5: Stick Parity bit
 Bit 4: Even parity bit
 Bit 3: Parity Enable bit
 Bit 2: Number of stop bit transmitted
 Bit 1:0: Number of data bits
 @param[in] unsigned char(uart_number)
 @param[in] unsigned long(Baud_rate)
 @param[in] unsigned long(frame_value)
 @param[in] unsigned long(Uart_clock)
 @param[Out] No ouput parameter.
 @return Void function.
*/
void uart_configure(UC uart_number, UL Baud_rate, UL frame_value, UL Uart_clock) {
	UL divisor;
	divisor = (Uart_clock / (Baud_rate * 16));
	UartReg(uart_number).UART_LCR = frame_value; //0x83
	UartReg(uart_number).UART_DR = divisor & 0xFF; //LSB
	UartReg(uart_number).UART_IE = (divisor >> 0x08) & 0xFF; //MSB(right shift)
	UartReg(uart_number).UART_LCR &= 0x7f; //DLAB bit to zero
	UartReg(uart_number).UART_IE = 0x01;
	UartReg(uart_number).UART_IIR_FCR = 0x00;
	__asm__ __volatile__ ("fence");
}


/**
 @fn void uart_putchar(UC uart_number, UC bTxCharacter, char *error)
 @brief 1 byte character transmission
 @details Transmit 1 character through uart.Proceeds only when transmitter is idle
 @param[in] unsigned char(uart_number-- which uart to be used)
 @param[in] unsigned char(bTxCharacter--character to be transmitted)
 @param[Out] unsigned char *error : if parity error -1, if Overrun error -2, if framing error -3
 no error 0
 @return Void function.
*/
void uart_putchar(UC uart_number, UC bTxCharacter, char *error) {

	while ((UartReg(uart_number).UART_LSR & 0x20) != 0x20
			&& (UartReg(uart_number).UART_LSR & 0x40) != 0x40)
		; //checks whether transmitter id idle and transmitter holding register is empty , to start transmitting
	//printf("ready to txmt \n\r");
	UartReg(uart_number).UART_DR = bTxCharacter;
	__asm__ __volatile__ ("fence");
	while ((UartReg(uart_number).UART_LSR & 0x20) != 0x20)
		;
	//printf("\n tx complete \n\r");
	if ((UartReg(uart_number).UART_LSR & 0x04) == 0x04) {
			//printf("ParityError\n\r");
			*error = UART_PARITY_ERROR;
		} else if ((UartReg(uart_number).UART_LSR & 0x02) == 0x02) {
			//printf("OverrunError\n\r"); //data in buffer is not read before next character was transfered and in FIFO, exceeds the trigger level
			*error = UART_OVERRUN_ERROR;
		} else if ((UartReg(uart_number).UART_LSR & 0x08) == 0x08) {
			//printf("FrammingError\n\r"); //frame does not match with the settings
			*error = UART_FRAMING_ERROR;
		} else
			*error = UART_NO_ERROR; //no error
}


/**
 @fn UC uart_getchar(UC uart_number, char *error)
 @brief 1 byte character reception
 @details Receives 1 character thcrough uart
 @param[in] unsigned char(uart_number)
 @param[Out] unsigned char *error : if parity error -1, if Overrun error -2,
 if framing error -3 ,no error 0
 @return unsigned char Rxd_data--data received
*/
UC uart_getchar(UC uart_number, char *error) {
	UC Rxd_data;

	while ((UartReg(uart_number).UART_LSR & 0x01) != 0x01)
		; //waiting for data
	//printf("ready to receive \n\r");
	Rxd_data = UartReg(uart_number).UART_DR; //

	if ((UartReg(uart_number).UART_LSR & 0x04) == 0x04) {
		//printf("ParityError\n\r");
		*error = UART_PARITY_ERROR;
	} else if ((UartReg(uart_number).UART_LSR & 0x02) == 0x02) {
		//printf("OverrunError\n\r"); //data in buffer is not read before next character was transfered and in FIFO, exceeds the trigger level
		*error = UART_OVERRUN_ERROR;
	} else if ((UartReg(uart_number).UART_LSR & 0x08) == 0x08) {
		//printf("FrammingError\n\r"); //frame does not match with the settings
		*error = UART_FRAMING_ERROR;
	} else
		*error = UART_NO_ERROR; //no error
	return Rxd_data;
}


/**
 @fn UC uart_getchar(UC uart_number, char *error)
 @brief 1 byte character reception
 @details Receives 1 character thcrough uart
 @param[in] unsigned char(uart_number)
 @param[Out] unsigned char *error : if parity error -1, if Overrun error -2,
 if framing error -3 ,no error 0
 @return unsigned char Rxd_data--data received
*/
UC uart_getchar_async(UC uart_number, char *error) {
	UC Rxd_data;
	 clock_t start_time=0, end_time=0;
    	start_time =  get_time();
    	end_time = start_time + ((1000000*10)/0.025);

	while ((UartReg(uart_number).UART_LSR & 0x01) != 0x01){//waiting for data
		 start_time =  get_time();	
		if(start_time > end_time)
	   	  break;
	}
		 
	//printf("ready to receive \n\r");
	Rxd_data = UartReg(uart_number).UART_DR; //

	if ((UartReg(uart_number).UART_LSR & 0x04) == 0x04) {
		//printf("ParityError\n\r");
		*error = UART_PARITY_ERROR;
	} else if ((UartReg(uart_number).UART_LSR & 0x02) == 0x02) {
		//printf("OverrunError\n\r"); //data in buffer is not read before next character was transfered and in FIFO, exceeds the trigger level
		*error = UART_OVERRUN_ERROR;
	} else if ((UartReg(uart_number).UART_LSR & 0x08) == 0x08) {
		//printf("FrammingError\n\r"); //frame does not match with the settings
		*error = UART_FRAMING_ERROR;
	} else
		*error = UART_NO_ERROR; //no error
	return Rxd_data;
}


/**
 @fn UC uart_getchar(UC uart_number, char *error)
 @brief 1 byte character reception
 @details Receives 1 character thcrough uart
 @param[in] unsigned char(uart_number)
 @param[Out] unsigned char *error : if parity error -1, if Overrun error -2,
 if framing error -3 ,no error 0
 @return unsigned char Rxd_data--data received
*/
UC uart_getchar_sim_txn_rxn(UC uart_number_tx,UC uart_number_rx, char *error,char txdata) {
	UC Rxd_data;
	 UC Tx_flag = 1;
	while ((UartReg(uart_number_rx).UART_LSR & 0x01) != 0x01){//waiting for data
		 if(Tx_flag==1){
			 UartReg(uart_number_tx).UART_DR = txdata;
			 Tx_flag=0;
		 }

	}

	//printf("ready to receive \n\r");
	Rxd_data = UartReg(uart_number_rx).UART_DR; //

	if ((UartReg(uart_number_rx).UART_LSR & 0x04) == 0x04) {
		printf("ParityError\n\r");
		*error = UART_PARITY_ERROR;
	} else if ((UartReg(uart_number_rx).UART_LSR & 0x02) == 0x02) {
		printf("OverrunError\n\r"); //data in buffer is not read before next character was transfered and in FIFO, exceeds the trigger level
		*error = UART_OVERRUN_ERROR;
	} else if ((UartReg(uart_number_rx).UART_LSR & 0x08) == 0x08) {
		printf("FrammingError\n\r"); //frame does not match with the settings
		*error = UART_FRAMING_ERROR;
	} else
		*error = UART_NO_ERROR; //no error
	return Rxd_data;
}


/**
@fn void uart_intr_enable(UC uart_number, UC tx_intr, UC rx_intr)
@brief enable uart interrupts
@details enable the uart tx and rx interrupt
@param[in] unsigned char(uart_number)
@param[in] unsigned char(tx_intr)
@param[in] unsigned char(rx_intr)
@param[Out] No ouput parameter.
@return Void function.
*/
void uart_intr_enable(UC uart_number, UC tx_intr, UC rx_intr) {

	//UartReg(uart_number).UART_IE = ((rx_intr << 2) | (tx_intr << 1));
	UartReg(uart_number).UART_IE = ((tx_intr << 1) | (rx_intr));
	__asm__ __volatile__ ("fence");
}


/** @fn uart_intr_handler
 * @brief  Interrupt handler.
 * @details Reads uart controllers status register to distinguish which type of interrupt has occurred.
 * @warning
 * @param[in] unsigned char
 * @param[Out] Returns 1 if Tx intr occurs, 2 if Rx intr occurs.
*/
/*int uart_intr_handler(UC uart_number) {
	UC status = 0;

	status = UartReg(uart_number).UART_IIR_FCR;
	if((status & 0x02)==0x02) // uart TX intr occurred.
		return 1;
	else if((status & 0x04)==0x04)
		return 2;    // uart RX intr occurred.
}*/

