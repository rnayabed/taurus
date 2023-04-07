/***************************************************************************
 * Name of Author                        :  Anoop Varghese, C-DAC
 * Email ID  (Report any bugs)           :  vanoop@cdac.in

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

#include "ds18b20.h"
#include "stdlib.h"
#include "uart.h"



/**
 @file ds18b20_sensor.c
 @brief Contains routines for demonstrates the temperature monitoring system
 */
volatile US *gpioDirAddr = (volatile US *)(GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.

/* Sets 1-Wire GPIO pin in input mode.*/
void setWireInput(void)
{
  US dirData = *gpioDirAddr;
  dirData &= ~(1 << ONE_WIRE_GPIO);
  *gpioDirAddr = dirData;
  __asm__ __volatile__("fence");
}

/* Sets 1-Wire GPIO pin in output mode.*/
void setWireOutput(void)
{
  US dirData = *gpioDirAddr;
  dirData |= (1 << ONE_WIRE_GPIO);
  *gpioDirAddr = dirData;
  __asm__ __volatile__("fence");
}

/* Writes 'bit' (0 or 1) to  1-Wire GPIO pin.*/
void writeWire(US bit)
{
  UL gpioAddr;
  volatile US *gpioData;
  gpioAddr = GPIO_0_BASE_ADDRESS;          // Base address of GPIO 0.
  gpioAddr |= ((1 << ONE_WIRE_GPIO) << 2); // Generate address of the selected GPIO pin in GPIO 0.
  gpioData = (US *)gpioAddr;               // Generated address.
  *gpioData = (bit << ONE_WIRE_GPIO);
  __asm__ __volatile__("fence");
}

/* Reasds 'bit' (0 or 1) from 1-Wire GPIO pin.*/
US readWire(void)
{
  UL gpioAddr;
  volatile US *gpioData;
  gpioAddr = GPIO_0_BASE_ADDRESS;          // Base address of GPIO 0.
  gpioAddr += ((1 << ONE_WIRE_GPIO) << 2); // Generate address of the selected GPIO pin in GPIO 0.
  gpioData = (US *)gpioAddr;               // Generated address.
  __asm__ __volatile__("fence");
  if (*gpioData)
    return 1;
  return 0;
}

/* Sets 1-Wire GPIO pin to high (1).*/
void ds18b20High(void)
{
  setWireOutput(); //set port as output
  writeWire(HIGH); //set port high
}

/* Sets 1-Wire GPIO pin to low (0).*/
void ds18b20Low(void)
{
  setWireOutput(); //set port as output
  writeWire(LOW);  //set port low
}

/* Initialize ds18b20 temp sensor.*/
void ds18b20Init(void)
{

  setWireOutput(); //set data pin as output
  writeWire(1);    //set data pin to high
}

/* Reset ds18b20 temp sensor. */
unsigned int ds18b20Reset(void)
{
  int device_present = 0;
  ds18b20Low(); // Drive bus low
  /* DELAY [480 mirco second]*/
  for (int i = 0; i < 480; i++) // hold for 480us
    delay_1_us();
  setWireInput(); //release bus, set port in input mode
  /* DELAY [80 mirco second]*/
  for (int i = 0; i < 80; i++) //delay for 80us
    delay_1_us();

  if (readWire())
  {
    device_present = 1; // Device presence not found
    /* DELAY [480 mirco second]*/
    for (int i = 0; i < 480; i++) //delay for 480us
      delay_1_us();
  }
  else
  {
    device_present = 0; // Device presence found
    /* DELAY [480 mirco second]*/
    for (int i = 0; i < 480; i++)
      delay_1_us();
  }
  return device_present;
}

/* Write zero to 1-wire.*/
void writeWireZero(void)
{
  ds18b20Low(); // Drive bus low
  /* DELAY [60 mirco second]*/
  for (int i = 0; i < 60; i++) //sample time slot for the slave
    delay_1_us();
  setWireInput(); //release bus, set port in input mode
  /* DELAY [1 mirco second]*/
  delay_1_us(); //recovery time slot
}

/* Write one to 1-wire.*/
void writeWireOne(void)
{
  ds18b20Low(); // Drive bus low
  /* DELAY [5 mirco second]*/
  for (int i = 0; i < 5; i++)
    delay_1_us();
  setWireInput(); //release bus, set port in input mode
  /* DELAY [1 mirco second]*/
  delay_1_us(); //recovery time slot
  /* DELAY [60 mirco second]*/
  for (int i = 0; i < 60; i++) //sample time slot for the slave
    delay_1_us();
}

/* Write command to ds18b20 temp sensor.*/
void write_ds18b20(unsigned char data)
{
  // LSB is sent first
  unsigned char i;
  for (i = 8; i > 0; i--)
  {
    if (data & 0x01)
    {
      writeWireOne();
    }
    else
    {
      writeWireZero();
    }
    data >>= 1;
  }
}

/* Read one bit from 1-wire.*/
unsigned int readWireBit(void)
{
  int bit = 0;
  ds18b20Low(); // Drive bus low
  /* DELAY [1 mirco second]*/
  delay_1_us();   //hold for 1us
  setWireInput(); //release bus, set port in input mode
  /* DELAY [10 mirco second]*/
  for (int i = 0; i < 10; i++) //wait for slave to drive port either high or low
    delay_1_us();

  if (readWire()) //read bus
  {
    bit = 1; //if read high set bit high
  }
  else
  {
    bit = 0;
  }
  /* DELAY [60 mirco second]*/
  for (int i = 0; i < 60; i++) //recovery time slot
    delay_1_us();

  return bit;
}

/* Read 16 bit data from 1-wire.*/
unsigned int read_ds18b20(void)
{
  unsigned char i;
  unsigned int data = 0;

  setWireInput(); //release bus, set port in input mode
  for (i = 16; i > 0; i--)
  {
    data >>= 1;
    if (readWireBit())
    {
      data |= 0x8000; //LSB is sent first
    }
  }
  return (data);
}

/* Get get raw temperature from ds18b20 temp sensor.*/
int getRawTemperature(void)
{
  int temperature;
  unsigned char ds18b20Presence = 0;
  ds18b20Presence = ds18b20Reset(); //check for device presence by reset command.
  if (!ds18b20Presence)             // if temperature sensor was found, do the rest of processing.
  {
    write_ds18b20(DS18S20_SKIP_ROM);  // write Skip ROM command to DS18S20.
    write_ds18b20(DS18S20_CONVERT_T); // write convert temperature command to DS18S20.
    /* DELAY [5 mirco second]*/
    for (int i = 0; i < 5; i++)
      delay_1_us();
    /* DELAY [750 milli second]*/
    for (int i = 0; i < 780000; i++) // wait for temperature conversion (750ms).
      delay_1_us();
    /* DELAY [5 mirco second]*/
    for (int i = 0; i < 5; i++)
      delay_1_us();
    ds18b20Reset();                         // Reset DS18S20.
    write_ds18b20(DS18S20_SKIP_ROM);        // write Skip ROM command to DS18S20.
    write_ds18b20(DS18S20_READ_SCRATCHPAD); //write command to read scratch pad ROM.
    temperature = read_ds18b20();           //Read the data.
    return (temperature & TEMP_DATA_MASK);
    //return temperature;
  }
  else
  {
    return DS18B20_NOT_FOUND;
  }
}


