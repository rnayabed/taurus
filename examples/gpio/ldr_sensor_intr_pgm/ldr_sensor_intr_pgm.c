

/***************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  LDR Sensor Demo
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

#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include "interrupt.h"

/**
 @file ldr_sensor_intr_pgm.c
 @brief Main program to check the status of LDR sensor
*/

/** @fn gpio_intr_handler_ldr
  @brief  gpio 0 intr handler.
  @details The function will execute the steps as described in routine
  @param[in] unsigned char, unsigned int
  @param[Out] No output parameter.
*/

void gpio_intr_handler_ldr(void) {

	printf("\n\nLight Status : Absent  ( )");
	return;   
}

/** 
  @brief  to check the status of LDR sensor
  @details 
*/
void main()
{
	printf("+-----[ LDR Sensor Demo ]-----+\n");
	printf("|                             |\n");
	printf("|    sensor DO ---> GPIO0     |\n");
	printf("|                             |\n");
	printf("+-----------------------------+\n\n");

	irq_register_handler(GPIO_0_IRQ, gpio_intr_handler_ldr);

	interrupt_enable(GPIO_0_IRQ);
	
	while (1);
	
}
