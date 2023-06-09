/**
 @file config.h
 @brief header file for hardware configuration registers'
 @authors Debayan Sutradhar (@rnayabed), Avra Mitra (@abhra0897)
*/

#ifndef CONFIG_H
#define CONFIG_H

/* Special Function Register defs */
#define _SFR_32(mem_addr)	(*(volatile unsigned long *)(mem_addr))
#define _SFR_16(mem_addr)	(*(volatile unsigned short *)(mem_addr))
#define _SFR_8(mem_addr)	(*(volatile unsigned char *)(mem_addr))

#define SYSTEM_FREQUENCY 			100000000UL
#define UART_BASE				    0x10000100UL
#define ADC_BASE_ADDRESS 	        0x10001000UL
#define SPI_BASE_ADDR(i)  			(((i) == 0 || (i) == 1) ? 0x10000600UL : 0x10200100UL)
#define I2C_BASE_ADDR				0x10000800UL

#define GPIO_PORT_0_BASE_ADDRESS    0x10080000UL
#define GPIO_PORT_0_DDR_ADDRESS     0x100C0000UL
#define GPIO_PORT_0_DDR             _SFR_16(GPIO_PORT_0_DDR_ADDRESS)
#define GPIO_PORT_0_IO(io_num)      _SFR_16(GPIO_PORT_0_BASE_ADDRESS | (1 << (io_num + 2)))
#define GPIO_PORT_0_PIN(pin_num)     GPIO_PORT_0_IO(pin_num)

#define GPIO_PORT_1_BASE_ADDRESS    0x10180000UL
#define GPIO_PORT_1_DDR_ADDRESS     0x101C0000UL
#define GPIO_PORT_1_DDR             _SFR_16(GPIO_PORT_1_DDR_ADDRESS)
#define GPIO_PORT_1_IO(io_num)      _SFR_16(GPIO_PORT_1_BASE_ADDRESS | (1 << (io_num + 2)))
#define GPIO_PORT_1_PIN(pin_num)     GPIO_PORT_1_IO(pin_num)

#define TIMER_BASE_ADDRESS 			0x10000A00UL
#define TIMER_INTR_STATUS_ADDRESS   0x10000AA8UL
#define TIMER_INTR_STATUS           _SFR_32(TIMER_INTR_STATUS_ADDRESS)
#define TIMER_INTR_FLAG(timer_num)  (TIMER_INTR_STATUS & (1 << (timer_num)))

#define PWMC_BASE_ADDR 				0x10400000UL

#define PLIC_BASE_ADDRESS           0x20010000UL
#define PLIC_SW_INTR_EN_ADDRESS     0x20010400UL
#define PLIC_SW_INTR_EN             _SFR_32(PLIC_SW_INTR_EN_ADDRESS)

#define CONCATENATE(X) #X
#define CONCAT(X) CONCATENATE(X)

#endif /* CONFIG_H */
