/** @file gpio.h
 *  @brief Header file with functions to use GPIO pins.
 *
 *  @author Debayan Sutradhar
 *
 *  SPDX-License-Identifier: MIT
 */



#ifndef GPIO_H
#define GPIO_H

#define GPIO_0			0
#define GPIO_1			1
#define GPIO_2			2
#define GPIO_3			3

#define GPIO_4			4
#define GPIO_5			5
#define GPIO_6			6
#define GPIO_7			7

#define GPIO_8			8
#define GPIO_9			9
#define GPIO_10			10
#define GPIO_11			11

#define GPIO_12			12
#define GPIO_13			13
#define GPIO_14			14
#define GPIO_15			15

#define GPIO_16			16
#define GPIO_17			17
#define GPIO_18			18
#define GPIO_19			19

#define GPIO_20			20
#define GPIO_21			21
#define GPIO_22			22
#define GPIO_23			23

#define GPIO_24			24
#define GPIO_25			25
#define GPIO_26			26
#define GPIO_27			27

#define GPIO_28			28
#define GPIO_29			29
#define GPIO_30			30
#define GPIO_31			31

#define HIGH		    1
#define LOW			    0

#define OUT             1
#define IN              0


void GPIO_init();
void GPIO_set_pin_mode(unsigned short pin, unsigned short direction);
unsigned short GPIO_read_pin(unsigned short pin);
void GPIO_write_pin(unsigned short pin, unsigned short data);
//unsigned long long pulse_duration();
//UL pulse_duration(US GPIO_number, US val);


#endif /* GPIO_H */
