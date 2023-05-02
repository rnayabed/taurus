/**
 @file config.h
 @brief header file for hardware configuration registers
*/

#ifndef CONFIG_H
#define CONFIG_H

#define SYSTEM_FREQUENCY 			100000000
#define UART_BASE				    0x10000100UL
#define ADC_BASE_ADDRESS 	        0x10001000UL
#define SPI_BASE_ADDR(i)  			(((i) == 0 || (i) == 1) ? 0x10000600UL : 0x10200100UL)
#define I2C_BASE_ADDR				0x10000800UL

#define GPIO_PORT_0_BASE_ADDRESS    0x10080000UL
#define GPIO_PORT_0_DDR_ADDRESS     0x100C0000UL
#define GPIO_PORT_0_DDR             _SFR_16(GPIO_PORT_0_DDR_ADDRESS)
#define GPIO_PORT_0_IO(io_num)      _SFR_16(GPIO_PORT_0_BASE_ADDRESS | (1 << (pin_num + 2)))

#define GPIO_PORT_1_BASE_ADDRESS    0x10180000UL
#define GPIO_PORT_1_DDR_ADDRESS     0x101C0000UL
#define GPIO_PORT_1_DDR             _SFR_16(GPIO_PORT_1_DDR_ADDRESS)
#define GPIO_PORT_1_IO(io_num)      _SFR_16(GPIO_PORT_1_BASE_ADDRESS | (1 << (pin_num + 2)))

#define TIMER_BASE_ADDRESS 			0x10000A00UL
#define TIMER_INTR_STATUS_ADDRESS   0x10000AA8UL

#define PWMC_BASE_ADDR 				0x10400000UL

#define CONCATENATE(X) #X
#define CONCAT(X) CONCATENATE(X)

#endif /* CONFIG_H */
