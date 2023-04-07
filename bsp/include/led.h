
#ifndef LED_H_
#define LED_H_

/**
 @file led.h
 @brief header file for LED functions 
 @detail 
 */

/*  Include section
*
***************************************************/


/*  Defines section
*


***************************************************/
#define OFF_LED		1
#define ON_LED			0
#define OFF_LED_ARIES 	0
#define ON_LED_ARIES	1


/*  Function declarations
*
***************************************************/
void on_LED(US pin_no);
void off_LED(US pin_no);
void on_LED_RGB(US pin_no);
void off_LED_RGB(US pin_no);
US read_switch(US pin_no);

#endif /* LED_H_ */
