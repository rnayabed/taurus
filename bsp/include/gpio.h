
#ifndef GPIO_H_
#define GPIO_H_


/**
 @file gpio.h
 @brief header file for GPIO functions 
 @detail 
 */
/*  Include section
*
***************************************************/


/*  Defines section
*


***************************************************/
#define PIN_0			0
#define PIN_1			1
#define PIN_2			2
#define PIN_3			3

#define PIN_4			4
#define PIN_5			5
#define PIN_6			6
#define PIN_7			7

#define PIN_8			8
#define PIN_9			9
#define PIN_10			10
#define PIN_11			11

#define PIN_12			12
#define PIN_13			13
#define PIN_14			14
#define PIN_15			15

#define PIN_16			16
#define PIN_17			17
#define PIN_18			18
#define PIN_19			19

#define PIN_20			20
#define PIN_21			21
#define PIN_22			22
#define PIN_23			23

#define PIN_24			24
#define PIN_25			25
#define PIN_26			26
#define PIN_27			27

#define PIN_28			28
#define PIN_29			29
#define PIN_30			30
#define PIN_31			31

#define GPIO_0			0
#define GPIO_1			1

#define GPIO_2			2
#define GPIO_3			3

#define ON_LED			0
#define OFF_LED			1

#define ON			    1
#define OFF			    0

#define HIGH		    1
#define LOW			    0

#define IN              0
#define OUT             1

/*  Function declarations
*
***************************************************/

void GPIO_init();
void GPIO_set_pin_mode(int pin, int direction);
int GPIO_read_pin(int pin);
void GPIO_write_pin(int pin,int data);
//UL pulse_duration(US pin_number, US val);

#endif /* GPIO_H_ */
