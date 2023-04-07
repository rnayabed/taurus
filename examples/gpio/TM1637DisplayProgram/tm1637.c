/***************************************************************************
* Module name                           :  tm1637.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  TM1637 7-Segment Display 

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
/**
 @file tm1637.c
 @brief functions for for 7-segment display
*/
#include "tm1637.h"
#include "stdlib.h"
#include "uart.h"
#include "gpio.h"

volatile US *gpioDirAddr1 = (volatile US *)(GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.
volatile US *gpioDirAddr2 = (volatile US *)(GPIO_1_BASE_ADDRESS + 0x40000); // GPIO 1 direction register.

/* Sets GPIO pin in input mode.*/
void setWireInput(unsigned char gpioPin)
{
  unsigned short bitPos, dirData;

  if (gpioPin > 15)
  {
    bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
    dirData = *gpioDirAddr2;        // Address of the direction register.
    dirData &= ~(bitPos);           // Clearing a bit configures the pin to be INPUT.
    *gpioDirAddr1 = dirData;        // Data written to direction register.
    __asm__ __volatile__("fence");
  }
  else
  {
    bitPos = (1 << gpioPin); // Align the selected pin to its position.
    dirData = *gpioDirAddr1; // Address of the direction register.
    dirData &= ~(bitPos);    // Clearing a bit configures the pin to be INPUT.
    *gpioDirAddr1 = dirData; // Data written to direction register.
    __asm__ __volatile__("fence");
  }
}

/* Sets GPIO pin in output mode.*/
void setWireOutput(unsigned char gpioPin)
{
  unsigned short bitPos, dirData;

  if (gpioPin > 15)
  {
    bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
    dirData = *gpioDirAddr2;        // Address of the direction register.
    dirData |= (bitPos);            // Clearing a bit configures the pin to be INPUT.
    *gpioDirAddr1 = dirData;        // Data written to direction register.
    __asm__ __volatile__("fence");
  }
  else
  {
    bitPos = (1 << gpioPin); // Align the selected pin to its position.
    dirData = *gpioDirAddr1; // Address of the direction register.
    dirData |= (bitPos);     // Clearing a bit configures the pin to be INPUT.
    *gpioDirAddr1 = dirData; // Data written to direction register.
    __asm__ __volatile__("fence");
  }
}

/* Delay for tm1637.*/
void bitDelay(void)
{
  udelay(2);
}

/* Write 8-Bit data to tm1637 sensor.*/
unsigned short writeByte(uint8_t data, tm1637 display)
{
  unsigned short ack;

  // 8 Data Bits
  for (int i = 0; i < 8; i++)
  {
    // CLK low
    setWireOutput(display.clkPin);
    bitDelay();

    // Set data bit
    if (data & 0x01)
      setWireInput(display.DioPin);
    else
      setWireOutput(display.DioPin);

    bitDelay();

    // CLK high
    setWireInput(display.clkPin);
    bitDelay();
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  setWireOutput(display.clkPin);
  setWireInput(display.DioPin);
  bitDelay();

  // CLK to high
  setWireInput(display.clkPin);
  bitDelay();
  ack = GPIO_read_pin(display.DioPin);
  if (ack == 0)
    setWireOutput(display.DioPin);

  bitDelay();
  setWireOutput(display.clkPin);
  bitDelay();

  return ack;
}

void startTM1637(tm1637 display)
{
  setWireOutput(display.DioPin);
  bitDelay();
}

void stopTM1637(tm1637 display)
{
  setWireOutput(display.DioPin);
  bitDelay();
  setWireInput(display.clkPin);
  bitDelay();
  setWireInput(display.DioPin);
  bitDelay();
}

void clearTM1637(tm1637 display)
{
  uint8_t data[] = {0, 0, 0, 0};
  setSegments(data, display);
}

void setSegments(const uint8_t segments[], tm1637 display)
{
  uint8_t length = 4; // FIXME : make me function argument.
  uint8_t pos = 0;    // FIXME : make me function argument.
                      // Write COMM1
  startTM1637(display);
  writeByte(TM1637_I2C_COMM1, display);
  stopTM1637(display);

  // Write COMM2 + first digit address
  startTM1637(display);
  writeByte(TM1637_I2C_COMM2 + (pos & 0x03), display);

  // Write the data bytes
  for (uint8_t k = 0; k < length; k++)
    writeByte(segments[k], display);

  stopTM1637(display);

  // Write COMM3 + brightness
  startTM1637(display);
  writeByte(TM1637_I2C_COMM3 + (display.brightness & 0x0f), display);
  stopTM1637(display);
}

void setDots(uint8_t *string)
{
  string[1] |= 0x80;
}
void displayString(tm1637 display, uint8_t *string, int length)
{
  uint8_t encodedData;
  const uint8_t digits[] = {
      // XGFEDCBA
      0b00111111, // 0
      0b00000110, // 1
      0b01011011, // 2
      0b01001111, // 3
      0b01100110, // 4
      0b01101101, // 5
      0b01111101, // 6
      0b00000111, // 7
      0b01111111, // 8
      0b01101111, // 9
  };
  const uint8_t alphabets[] = {0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, 0x1E, 0x00, 0x38, 0x00, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x00, 0x00, 0x00, 0x6E, 0x00};
  uint8_t data[] = {0x0, 0x0, 0x0, 0x0};
  for (int index = 0; index < (length + 4); index++) /* length + 4 is to clear animation*/
  {
    if (index < length)
    {
      if (string[index] >= '0' && string[index] <= '9')
      {
        encodedData = digits[string[index] - '0'];
      }
      else if (string[index] >= 'A' && string[index] <= 'Z')
      {
        encodedData = alphabets[string[index] - 'A'];
      }
      else if (string[index] >= 'a' && string[index] <= 'z')
      {
        encodedData = alphabets[string[index] - 'a'];
      }
      else
      {
        encodedData = 0x0;
      }
    }
    else
    {
      encodedData = 0;
    }

    data[0] = data[1];
    data[1] = data[2];
    data[2] = data[3];
    data[3] = encodedData;
    udelay(ANIMATION_DELAY); /*animation delay*/
    setSegments(data, display);
  }
}
