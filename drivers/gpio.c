/** @file gpio.c
 *  @brief Contains functions to use GPIO pins.
 *
 *  @author Debayan Sutradhar
 *
 *  SPDX-License-Identifier: MIT
 */


#include "gpio.h"
#include "config.h"


/** @fn void GPIO_init(void)
 *  @brief Initialises GPIO pins.
 *
 *  Initialises all GPIO pins as OUTPUT LOW,
 *  except for Pin 22, 23, 24 which are initialised as OUTPUT HIGH.
 */
void GPIO_init(void)
{
    // Set all pins as output
    *((unsigned short*) GPIO_0_DDR_ADDRESS) = 0xffff;
    *((unsigned short*) GPIO_1_DDR_ADDRESS) = 0xffff;

    // Set Pin 22, 23 and 24 as HIGH, others as low
    // FIXME: This should be done only when building for Aries v2.0
    *((unsigned short*) (GPIO_1_BASE_ADDRESS | (1 << 8))) = 1 << 6;
    *((unsigned short*) (GPIO_1_BASE_ADDRESS | (1 << 9))) = 1 << 7;
    *((unsigned short*) (GPIO_1_BASE_ADDRESS | (1 << 10))) = 1 << 8;
}


/** @fn void GPIO_set_pin_mode(unsigned short pin, unsigned short direction)
 *  @brief Set data direction of pin.
 *  @param pin GPIO pin.
 *  @param direction Direction of the pin.
 */
void GPIO_set_pin_mode(unsigned short pin, unsigned short direction)
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
        *((volatile unsigned short*) adr) &= ~(1 << pin);
    }
    else
    {
        *((volatile unsigned short*) adr) |= 1 << pin;
    }

    __asm__ __volatile__ ("fence");
}


/** @fn int GPIO_read_pin(unsigned short pin)
 *  @brief Read GPIO pin value.
 *  @param pin GPIO pin.
 *  @return Value read from the pin.
 */
unsigned short GPIO_read_pin(unsigned short pin)
{
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

    return *((volatile unsigned short*) (adr + (1 << (pin + 2))));
}


/** @fn void GPIO_write_pin(unsigned short pin, unsigned short data)
 *  @brief Write value to GPIO pin.
 *  @param pin GPIO pin.
 *  @param data Data to be written to pin.
 */
void GPIO_write_pin(unsigned short pin, unsigned short data)
{
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

    *((volatile unsigned short*) (adr | (1 << (pin + 2)))) = data << pin;

    __asm__ __volatile__ ("fence");
}

/** @fn unsigned long long pulse_duration(unsigned short pin, unsigned short value)
 *  @brief Reads pulse from a GPIO pin.
 *  @param pin GPIO pin.
 *  @param pin value value to pulse on.
 */
/*unsigned long long pulse_duration(US pin_number, US val)
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
