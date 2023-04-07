//Vibration Motor Demo
/***************************************************
* Module name                           :  main.c
* Name of Author                        :  Maitreya Kale, C-DAC
* Email ID  (Report any bugs)           :  ramakant.km@cdac.in
* Module Description                    :  Vibration motor v1.3 module Demo
*
* Copyright 2020 Company CDAC(T)
* All Rights Reserved.
*
*  The information contained herein is confidential
* property of Company. The user, copying, transfer or
* disclosure of such information is prohibited except
* by express written agreement with Company.
*
***************************************************/

/*  Include section
*
*
***************************************************/
/**
 @file vibration_pgm.c
 @brief Main program to test the vibration motor v1.3 module.
*/
#include"stdlib.h"
#include"config.h"
#include"gpio.h"
#include"time.h"

#define MOTOR_PIN   3

void main()
{      
     printf("+--------[Vibration Motor v1.3 module Demo]------+\n");
     printf("|                                                |\n");
     printf("|                 sensor SIG -> GPIO3            |\n");
     printf("|                                                |\n");
     printf("+------------------------------------------------+\n");
    while(1)
    {
        GPIO_write_pin(MOTOR_PIN,HIGH);
        udelay(50000);
        GPIO_write_pin(MOTOR_PIN,LOW);
        udelay(50000);
    
    }
}
