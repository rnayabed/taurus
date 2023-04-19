#include <stdlib.h>
#include <gpio.h>
#include <config.h>

/**
 @file gpio.c
 @brief Contains routines to read/write GPIO pins
 */

void GPIO_init()
{
	// Setup all pins as output
	*((unsigned short*) GPIO_0_DDR_ADDRESS) = 0xffff;
    *((unsigned short*) GPIO_1_DDR_ADDRESS) = 0xffff;

	// Except Pin 22, 23, 24 (for CDAC Aries v2.0 active low on-board LEDs)
	// FIXME: This should be done only when building for Aries v2.0
	*((unsigned short*) (GPIO_1_BASE_ADDRESS | (1 << 8))) = 1 << 6;
	*((unsigned short*) (GPIO_1_BASE_ADDRESS | (1 << 9))) = 1 << 7;
	*((unsigned short*) (GPIO_1_BASE_ADDRESS | (1 << 10))) = 1 << 8;
}

/** @fn void GPIO_set_pin_mode(int pin,int data) 
 * @brief  Write GPIO pin value.
 * @details Write the GPIO pin value by setting the direction as OUTPUT.
 * @param[in] int, int
 * @return No output parameter.
 */
void GPIO_set_pin_mode(int pin, int direction)
{
	int adr;
	
	if (pin >= 16)
	{
		adr = GPIO_1_DDR_ADDRESS;
		pin -= 16;
	}
	else
	{
		adr = GPIO_0_DDR_ADDRESS;
	}

	if (direction == IN)
	{
		*((unsigned short*) GPIO_1_DDR_ADDRESS) &= ~(1 << pin);
	}
	else
	{
		*((unsigned short*) GPIO_1_DDR_ADDRESS) |= 1 << pin;
	}

	__asm__ __volatile__ ("fence");
}

/** @fn int GPIO_read_pin(int pin)
  @brief  Read GPIO pin value.
  @details Read the GPIO pin value by setting the direction as INPUT in GPIO direction register.
  @param[in] int
  @return Pin value as integer.
 */
int GPIO_read_pin(int pin)
{
	GPIO_set_pin_mode(pin, IN);

	int adr;
	
	if (pin >= 16)
	{
		adr = GPIO_1_BASE_ADDRESS;
		pin -= 16;
	}
	else
	{
		adr = GPIO_0_BASE_ADDRESS;
	}

	return *((unsigned short*) (adr + (1 << pin + 2)));
}

/** @fn void GPIO_write_pin(US pin_no,US data) 
 * @brief  Write GPIO pin value.
 * @details Write the GPIO pin value by setting the direction as OUTPUT.
 * @param[in] int, int
 * @return No output parameter.
 */
void GPIO_write_pin(int pin, int data)
{
	GPIO_set_pin_mode(pin, OUT);

	int adr;
	
	if (pin >= 16)
	{
		adr = GPIO_1_BASE_ADDRESS;
		pin -= 16;
	}
	else
	{
		adr = GPIO_0_BASE_ADDRESS;
	}

	*((unsigned short*) (adr | (1 << pin + 2))) = data << pin;
	__asm__ __volatile__ ("fence");
}


/** @fn UL pulse_duration(US pin_number, US val)
 * @brief  To find the duration of pulse
 * @details 
 * @param[in] unsigned short, pin number
 * @param[in] unsigned short, value
 * @return Pulse duration
*/
/*UL pulse_duration(US pin_number, US val)
{
	clock_t start_time=0, end_time=0;
	UL total_time=0;
	while(GPIO_read_pin(pin_number)!=val);
	start_time =  get_time();
	while(GPIO_read_pin(pin_number)==val);
	end_time =  get_time();	

	total_time = (end_time - start_time)*0.025;
	return total_time;
}*/



