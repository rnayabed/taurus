/***************************************************************************
 * Project                               :  MDP
 * Name of the file                      :  gpio.c
 * Brief Description of file             :  Driver to control the GPIO.
 * Name of Author                        :  Sreeju G R
 * Email ID                              :  sreeju@cdac.in

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


#include <include/stdlib.h>
#include <include/gpio.h>
#include <include/config.h>

/**
 @file gpio.c
 @brief Contains routines to read/write GPIO pins
 */


/** @fn US GPIO_read_pin(US pin_no)
  @brief  Read GPIO pin value.
  @details Read the GPIO pin value by setting the direction as INPUT in GPIO direction register.
  @param[in] unsigned short pin_no
  @return Pin value as 16 bit data.
*/
US GPIO_read_pin(US pin_no) {

	UC gpio_number = 0;
	US dir_data = 0;
	US bit_position = 0;
	volatile US *gpio_0_dir_addr = (volatile US *)(GPIO_0_BASE_ADDRESS +0x40000) ; // GPIO 0 direction register.
	volatile US *gpio_1_dir_addr = (volatile US *)(GPIO_1_BASE_ADDRESS +0x40000) ; // GPIO 1 direction register.

	UL gen_gpio_0_addr,gen_gpio_1_addr;
	volatile US *gpio_0_data, *gpio_1_data;
	US read_data = 0;

	if(pin_no <= 15)
		gpio_number = 0; 			// Pins 0 to 15 defined in GPIO 0.
	else	
		gpio_number = 1;			// Pins 16 to 31 defined in GPIO 1.


	if(gpio_number == 0)
	{
		bit_position = (1 << pin_no);		// Align the selected pin to its position.
		dir_data =  *gpio_0_dir_addr; 		// Address of the direction register.
		dir_data &= ~(bit_position);		// Clearing a bit configures the pin to be INPUT.
		*gpio_0_dir_addr = dir_data;		// Data written to direction register.
		__asm__ __volatile__ ("fence");

		gen_gpio_0_addr = GPIO_0_BASE_ADDRESS;	// Base address of GPIO 0.
		gen_gpio_0_addr+=(bit_position << 2);	// Generate address of the selected GPIO pin in GPIO 0.
		gpio_0_data = (US *)gen_gpio_0_addr;	// Generated address.
		read_data = *gpio_0_data; 		// Read data from the address.
		
	}
	else if(gpio_number == 1)
	{
		pin_no = (pin_no - 16);
		bit_position = (1 << pin_no);		// Align the selected pin to its position.
		dir_data =  *gpio_1_dir_addr;		// Address of the direction register.
		dir_data &= ~(bit_position);		// Clearing a bit configures the pin to be INPUT.
		*gpio_1_dir_addr = dir_data;		// Data written to direction register.
		__asm__ __volatile__ ("fence");

		gen_gpio_1_addr = GPIO_1_BASE_ADDRESS;	// Base address of GPIO 1.
		gen_gpio_1_addr+=(bit_position << 2);	// Generate address of the selected GPIO pin in GPIO 1.
		gpio_1_data = (US *)gen_gpio_1_addr;	// Generated address.
		read_data = *gpio_1_data;		// Read data from the address.
	}
	__asm__ __volatile__ ("fence");

	if(read_data)
		return 1; 				// GPIO pin is High.
	else
		return 0; 				// GPIO pin is Low.
}

/** @fn void GPIO_write_pin(US pin_no,US data) 
 * @brief  Write GPIO pin value.
 * @details Write the GPIO pin value by setting the direction as OUTPUT.
 * @param[in] unsigned short, unsigned short
 * @return No output parameter.
*/
void GPIO_write_pin(US pin_no,US data) {
	
	UC gpio_number = 0;
	US dir_data = 0;
	US bit_position = 0;
	volatile US *gpio_0_dir_addr = (volatile US *)(GPIO_0_BASE_ADDRESS +0x40000) ; // GPIO 0 direction register.
	volatile US *gpio_1_dir_addr = (volatile US *)(GPIO_1_BASE_ADDRESS +0x40000) ; // GPIO 1 direction register.
	UL gen_gpio_0_addr,gen_gpio_1_addr;
	volatile US *gpio_0_data, *gpio_1_data;

	if(pin_no <= 15)
		gpio_number = 0;			// Pins 0 to 15 defined in GPIO 0.
	else	
		gpio_number = 1;			// Pins 16 to 31 defined in GPIO 1.	

	if(gpio_number == 0)
	{
		bit_position = (1 << pin_no);			// Align the selected pin to its position.
		dir_data =  *gpio_0_dir_addr;	 		// Address of the direction register.
		dir_data |= bit_position;			//Setting a bit configures the pin to be OUTPUT.
		*gpio_0_dir_addr = dir_data;			// Data written to direction register.
		__asm__ __volatile__ ("fence");

		gen_gpio_0_addr = GPIO_0_BASE_ADDRESS;		// Base address of GPIO 0.
		gen_gpio_0_addr|=(bit_position << 2);		// Generate address of the selected GPIO pin in GPIO 0.
		gpio_0_data = (US *)gen_gpio_0_addr;		// Generated address.
		*gpio_0_data = (data << pin_no);		// Write data to the address.
	}
	else if(gpio_number == 1)
	{
		pin_no = (pin_no - 16);
		bit_position = (1 << pin_no);			// Align the selected pin to its position.
		dir_data =  *gpio_1_dir_addr;			// Address of the direction register.
		dir_data |= bit_position;			//Setting a bit configures the pin to be OUTPUT.
		*gpio_1_dir_addr = dir_data;			// Data written to direction register.
		__asm__ __volatile__ ("fence");

		gen_gpio_1_addr = GPIO_1_BASE_ADDRESS;		// Base address of GPIO 1.
		gen_gpio_1_addr|=(bit_position << 2);		// Generate address of the selected GPIO pin in GPIO 0.
		gpio_1_data = (US *)gen_gpio_1_addr;		// Generated address.
		*gpio_1_data = (data << pin_no);		// Write data to the address.
	}
	__asm__ __volatile__ ("fence");

	return;   
}

/** @fn UL pulse_duration(US pin_number, US val)
 * @brief  To find the duration of pulse
 * @details 
 * @param[in] unsigned short, pin number
 * @param[in] unsigned short, value
 * @return Pulse duration
*/
UL pulse_duration(US pin_number, US val)
{
	clock_t start_time=0, end_time=0;
	UL total_time=0;
	while(GPIO_read_pin(pin_number)!=val);
	start_time =  get_time();
	while(GPIO_read_pin(pin_number)==val);
	end_time =  get_time();	

	total_time = (end_time - start_time)*0.025;
	return total_time;
}



