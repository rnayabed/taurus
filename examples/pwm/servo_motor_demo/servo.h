/***************************************************/

/*  Include section
*
*
***************************************************/
#ifndef SERVO_H_
#define SERVO_H_
#include "pwmc.h"
#define SERVO_DELAY	500
#define SERVO_MIN	20000
#define SERVO_MAX	100000
#define SERVO_PERIOD	1500000 //For 50Hz, thejas soc 40mHz, period=CPU_FREQ/HZ

void servoRotate(int pwmPin, int angle);

#endif /*SERVO_H_*/

