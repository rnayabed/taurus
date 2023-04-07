/***************************************************


 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Nov-2019
 Filename		: i2c.c
 Purpose		: I2C Firmware
 Description		: I2C routines
 Author(s)		: Karthika P
 Email			: karthikap@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file i2c.c
 @brief Contains routines for I2C interface
 @detail Includes software functions to initialize,
 configure, write and read over I2C interface
 */
#include <include/stdlib.h>
#include <include/i2c.h>
#include <include/config.h>

UL SYS_CLK;
UL I2C_CLK;

/**
 @fn void i2c_initialize(UC i2c_number)
 @brief I2C Initialization
 @details Initializes I2C Clock period registers
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[Out] No ouput parameter.
 @return Void function.

 */
void i2c_initialize(UC i2c_number) {
	I2CReg(i2c_number).I2C_TXCLR = 0xFF;

	__asm__ __volatile__ ("fence");
	US CH = SYS_CLK / (2 * I2C_CLK);
	I2CReg(i2c_number).I2C_CHL = ((UC) (CH & 0xFF));
	//printf("CHL is %x", (I2CReg(i2c_number).I2C_CHL));
	//printf("\n\r");
	I2CReg(i2c_number).I2C_CHH = (UC) ((CH >> 8) & 0xFF);
	//printf("CHH is %x", (I2CReg(i2c_number).I2C_CHH));
	//printf("\n\r");
	US CHH = SYS_CLK / (4 * I2C_CLK);

	I2CReg(i2c_number).I2C_CHHL = ((UC) (CHH & 0xFF));
	//printf("CHHL is %x", (I2CReg(i2c_number).I2C_CHHL));
	//printf("\n\r");
	I2CReg(i2c_number).I2C_CHHH = (UC) ((CHH >> 8) & 0xFF);
	//printf("CHHH is %x", (I2CReg(i2c_number).I2C_CHH));
	//printf("\n\r");
	__asm__ __volatile__ ("fence");
}

/**
 @fn UC i2c_start(UC i2c_number, UC read_length, UC Read)
 @brief Generates Start Sequence for I2C
 @details Start sequence is setting start bit and read length in I2C control
 register for I2C operations.
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[in] unsigned char (read_length).
 @param[in] unsigned char (Read).1 if Read operation and 0 if write operation
 @param[Out] No output parameters.
 @return -2 if read length is greater than 16 and return 0 when success
 */

UC i2c_start(UC i2c_number, UC read_length, UC Read) {
	if (read_length > 16) {
		printf("max read length supported is 16 \n\r");
		return -2; //max read length 16 is supported
	}
	I2CReg(i2c_number).I2C_TXCLR = 0xFF;	//clear TxFifo
	while (((I2CReg(i2c_number).I2C_SR0 & 0x08) != 0x08)
			|| ((I2CReg(i2c_number).I2C_SR0 & 0x10) != 0x10))
		; //checks for transmission complete and TxFifo empty

	if (Read == 1)
		I2CReg(i2c_number).I2C_CR = ((read_length << 2) | 0x01); //Set Start bit for read;read_length=1 byte
	else
		I2CReg(i2c_number).I2C_CR = 0x01; //Set Start bit for write;

	__asm__ __volatile__ ("fence");
	//printf("CR is %x",I2CReg(i2c_number).I2C_CR);
	//printf("\n\r");
	while ((I2CReg(i2c_number).I2C_SR0 & 0x01) != 0x01)
		; //check start sequence initiated
	return 0;
}

/**
 @fn void i2c_configure(UC i2c_number, UL System_Clock, UL I2C_Clock) 
 @brief Configures I2C
 @details Configures I2C Clock period registers
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[in] unsigned short(System_Clock,I2C_Clock)
 @param[Out] No output parameters.
 @return nil
 */

void i2c_configure(UC i2c_number, UL System_Clock, UL I2C_Clock) {
	I2C_CLK = I2C_Clock;
	SYS_CLK = System_Clock;
	i2c_initialize(i2c_number);
}

/**
 @fn UC i2c_data_write(UC i2c_number, UC *write_data, UC write_length)
 @brief Write data to transmit Fifo and checks for NACK
 @details Data/address to be transmitted is written into transmit fifo and checks NACK
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[in] unsigned char(*write_data-->Data/Address to be transmitted)
 @param[in] unsigned char(*write_length-->Number of bytes to be written)
 @param[Out] No output parameters.
 @return 0 if ACK and 1 if NACK
 */

UC i2c_data_write(UC i2c_number, UC *write_data, UC write_length) {
	for (int i = 0; i < write_length; i++) {
		while ((I2CReg(i2c_number).I2C_SR0 & 0x04) == 0x04)
			; //waits if TxFF full

		I2CReg(i2c_number).I2C_TxFF = *write_data;
		write_data++;
		__asm__ __volatile__ ("fence");
	}

	while ((I2CReg(i2c_number).I2C_SR0 & 0x10) != 0x10)

		; //wait for Transfer complete

	if ((I2CReg(i2c_number).I2C_SR1 & 0x01) == 0x01) { //checks NACK

		while ((I2CReg(i2c_number).I2C_SR0 & 0x02) != 0x02)
			; //wait for stop bit to be set
		//printf("NACK rxd and stop bit set\n\r");
		return 1;
	} else
		return 0;
}

/**
 @fn void i2c_stop(UC i2c_number)
 @brief Generates Stop Sequence for I2C
 @details Stop sequence is setting stop bit I2C control register for I2C operations.
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[Out] No output parameters.
 @return Nil
 */

void i2c_stop(UC i2c_number) {

	I2CReg(i2c_number).I2C_CR = 0x02; //Set Stop bit;
	__asm__ __volatile__ ("fence");
	while ((I2CReg(i2c_number).I2C_SR0 & 0x02) != 0x02)
		; //check stop sequence initiated

}

/**
 @fn UC i2c_data_read(UC i2c_number, UC *read_data, UC read_length) 
 @brief Read  data from  I2C device
 @details After Rx Complete,read data from fifo till fifo is empty
 @param[in] unsigned char(i2c_number--which i2c to be used)
 @param[in] unsigned char(*read_data--data read from EEPROM)
 @param[in] unsigned char(read_length--no:of bytes to read)
 @param[Out] No output parameters.
 @return unsigned char (Data-->Data from I2C device)
 */

UC i2c_data_read(UC i2c_number, UC *read_data, UC read_length) {
	if (read_length > 16) {
		printf("max read length supported is 16 \n\r");
		return -2; //max read length 16 is supported
	}
	while ((I2CReg(i2c_number).I2C_SR0 & 0x80) != 0x80)
		; //checks for RX complete
	for (int i = 0; i < read_length; i++) {

		while ((I2CReg(i2c_number).I2C_SR0 & 0x40) == 0x40)
			;  //wait if  RXfifo empty
		*read_data = I2CReg(i2c_number).I2C_RxFF; //reading RXFF
		read_data++;
		//printf("data is %x",Data);
		//printf("\n\r");
	}

	while ((I2CReg(i2c_number).I2C_SR0 & 0x02) != 0x02)
		; //wait for stop bit to be set
	return 0;

}

/** @fn void I2C_enable_intr(UC i2c_number,UC tx_intr,UC rx_intr)
 * @brief  Enable I2C interrupts.
 * @details Enable the I2C Tx and Rx interrupt.

 * @param[in] unsigned char,unsigned char,unsigned char
 * @param[Out] No output parameter
*/
void I2C_enable_intr(UC i2c_number,UC tx_intr,UC rx_intr)
{
	I2CReg(i2c_number).I2C_IER = ((rx_intr << 2) | (tx_intr << 1)|01);
		__asm__ __volatile__ ("fence");
}

/** @fn int I2C_intr_handler(UC I2C_number)
 * @brief  Interrupt handler.
 * @details Reads I2C controllers status register to distinguish which type of interrupt has occurred.

 * @param[in] unsigned char
 * @param[Out] Returns 1 if Tx intr occurs, 2 if Rx intr occurs.
*/
int I2C_intr_handler(UC I2C_number) {
	UC status = 0;

	status = I2CReg(I2C_number).I2C_SR1;
	if((status & 0x02)==0x02) // I2C TX intr occurred.
		return 1;
	else if((status & 0x03)==0x03)
		return 2;    // I2C RX intr occurred.
}

