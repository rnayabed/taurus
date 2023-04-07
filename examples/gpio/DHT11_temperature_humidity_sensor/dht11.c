/***************************************************************************
 * Module name                           :  dht11.c
 * Name of Author                        : Karthika P C-DAC
 * Email ID  (Report any bugs)           : karthikap@cdac.in
 * Module Description                    : Temperature and humidity sensor

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
#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include "dht11.h"
#include "encoding.h"

int data[40];
UC tempi = 0;
UC tempd = 0;
UC humi = 0;
UC humd = 0;
UC parity = 0;

volatile US *gpioDirAddr = (volatile US *) (GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.

/* Sets 1-Wire GPIO pin in input mode.*/
void setWireInput(void) {
	US dirData = *gpioDirAddr;
	dirData &= ~(1 << ONE_WIRE_GPIO);
	*gpioDirAddr = dirData;
	__asm__ __volatile__("fence");
}

/* Sets 1-Wire GPIO pin in output mode.*/
void setWireOutput(void) {
	US dirData = *gpioDirAddr;
	dirData |= (1 << ONE_WIRE_GPIO);
	*gpioDirAddr = dirData;
	__asm__ __volatile__("fence");
}

/* Writes 'bit' (0 or 1) to  1-Wire GPIO pin.*/
void writeWire(US bit) {
	UL gpioAddr;
	volatile US *gpioData;
	gpioAddr = GPIO_0_BASE_ADDRESS;          // Base address of GPIO 0.
	gpioAddr |= ((1 << ONE_WIRE_GPIO) << 2); // Generate address of the selected GPIO pin in GPIO 0.
	gpioData = (US *) gpioAddr;               // Generated address.
	*gpioData = (bit << ONE_WIRE_GPIO);
	__asm__ __volatile__("fence");
}

/* Reads 'bit' (0 or 1) from 1-Wire GPIO pin.*/
US readWire(void) {
	UL gpioAddr;
	volatile US *gpioData;
	gpioAddr = GPIO_0_BASE_ADDRESS;          // Base address of GPIO 0.
	gpioAddr += ((1 << ONE_WIRE_GPIO) << 2); // Generate address of the selected GPIO pin in GPIO 0.
	gpioData = (US *) gpioAddr;               // Generated address.
	__asm__ __volatile__("fence");
	if (*gpioData)
		return 1;
	return 0;
}

/* Sets 1-Wire GPIO pin to high (1).*/
void dht11High(void) {
	setWireOutput(); //set port as output
	writeWire(HIGH); //set port high
}

/* Sets 1-Wire GPIO pin to low (0).*/
void dht11Low(void) {
	setWireOutput(); //set port as output
	writeWire(LOW);  //set port low
}

/**
 * @brief Generation of Start Sequence
 * @details                                     .
 * @warning connect pullup resistor(4.7k)  gpio pin 1 with VDD.
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void dht11Start(void) {
	// Go into high impedence state to let pull-up raise data line level and
	// start the reading process.
	//dht11High();
	//Pull up the data pin prepare for the next step
	millisec_delay(250);
	// First set data line low for 20 milliseconds to trigger DHT11 to start generate result.
	// Page 5 in datasheet, it needs >18ms.
	dht11Low();
	millisec_delay(20);	//delay 20ms

	// End the start signal by setting data line high for 40 microseconds.
	microsec_delay(32);	//40us
	//dht11High();//Data pin DHT11 is pulled up

}
/**
 * @brief Reading Response signal
 * @details                                     .
 * @warning
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void dht11Response(void) {

	// Now start reading the data line to get the value from the DHT sensor.

	setWireInput();

	while (readWire())
		; //wait if HIGH
	//detects start signal and sent low response signal

	microsec_delay(67); //delay = 80us

	while (!readWire())
		; //wait if LOW

	//Prepares for Sending data and sent High response signal
	microsec_delay(67); //delay = 80us

}
/**
 * @brief Start data transmission
 * @details                                     .
 * @warning
 * @param[in] No input parameter
 * @param[Out] No output parameter
 * return 1 for bit =1 and 0 for bit=0
 */
int startTxnDHT(void) {

	while (readWire())
		; //wait if HIGH

	microsec_delay(41); //Delay 50us to let sensor pull data line low, and wait till low before get data.
	while (!readWire())
		; //wait if LOW
	microsec_delay(23); //delay =30 us
	if (readWire()) //data is 1 if data line is high for 70 us
		return 1;
	else
		return 0; //data is 1 if data line is high for 28us

}
/**
 * @brief Get data from DHT11 sensor
 * @details                                     .
 * @warning
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void GetDataDHT11() {
	for (int i = 0; i < 40; i++) {
		data[i] = startTxnDHT();

		if (data[i]) //if data is 1 data line is high for 70 us
			microsec_delay(32); //delay = 40us

	}

}
/**
 * @brief Get Humidity
 * @details  humi = humidity  integer form (bit0 to bit 7)
 *           humd = humidity  fractional part  (bit8 to bit 15)                               .
 * @warning
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void GetHumidityDHT11(void) {

	for (int i = 0; i < 8; i++) {
		humi *= 2;
		humi = humi + data[i]; //humi = humidity  integer form (bit0 to bit 7)
	}

	for (int i = 8; i < 16; i++) {
		humd *= 2;
		humd = humd + data[i]; // humd = humidity  fractional part  (bit8 to bit 15)
	}

}
/**
 * @brief Get Temperature
 * @details  tempi = temperature integer form (bit 16 to bit 23)
 *           tempd = temperature  fractional part  (bit 24 to bit 32)                               .
 * @warning
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void GetTemperatureDHT11(void) {

	for (int i = 16; i < 24; i++) {
		tempi *= 2;
		tempi = tempi + data[i]; // tempi = temperature integer form (bit 16 to bit 23)
	}

	for (int i = 24; i < 32; i++) {
		tempd *= 2;
		tempd = tempd + data[i]; // tempd = temperature integer form (bit 16 to bit 23)
	}

}
/**
 * @brief Get Checksum
 * @details  parity = bit 32 to bit 39
 *          if measurements are correct parity = humi + humd + tempi + tempd                            .
 * @warning
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void CheckSumDHT11(void) {

	for (int i = 32; i < 40; i++) {
		parity *= 2;
		parity = parity + data[i]; //parity = bit 32 to bit 39
	}
	if (parity != (humi + humd + tempi + tempd)) {
		printf("\n\r Checksum mismatch Incorrect Measurements");
		printf("\n\r Sensor Failure");
		while (1)
			;
	}

}

void main() {

	dht11Start();

	dht11Response();

	GetDataDHT11();

	GetHumidityDHT11();

	GetTemperatureDHT11();

	CheckSumDHT11();

	printf("\n\r Humidity in RH %d", humi);
	printf("\n\r Temperature in Celcius %d", tempi);
	printf("\n");

}

