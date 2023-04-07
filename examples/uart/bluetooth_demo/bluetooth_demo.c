/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Jul-2020
 Filename		: bluetooth_demo.c
 Purpose		: Bluetooth Demo
 Description		:  Bluetooth Demo with uart interface
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/
/**
 @file bluetooth_demo.c
 @brief Contains routines for demonstrates the bluetooth sensor communication
 */
 
/*  Include section
 ***************************************************/

#include "uart.h"
#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include <string.h>

int send_string(char * str)
{
char error;
  while(*str!='\0'){
	 uart_putchar(UART_1,*str, &error);
	if(*str=='\n')
	 uart_putchar(UART_1,'\r', &error);
	str++;
  }
  return 0;
}

void receive_string(char * str)
{
  char error;  
  char data;

  while(1){
	 data = uart_getchar(UART_1, &error);	
	 *str=data;
	  str++;
	 if(data=='\n'){	
	 *str='\r';	
	 break;
	}	
  }
}



/**
 @fn main
 @brief transmit and reception through uart
 @details 1 character is transmitted and received through uart
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
void main() {
	char error;       
	char str[100]={0,};

	printf("\n\r *****************************************************************************");
	printf("\n\r INFO: Connect HC-05 Bluetooth module to UART 1 ");	
	printf("\n\r INFO: Please try to connect the Bluetooth device 'HC-05'");	
	printf("\n\r INFO: Press the 'SCAN' button in VEGA bluetooth App from your Android phone");	
	printf("\n\r *****************************************************************************");

	printf("\n\r Setting up Bluetooth Device \n\r");
	uart_set_baud_rate(UART_1,9600,SYSTEM_FREQUENCY);

while(1){
		memset(str,0,sizeof(str));
		receive_string(str);	
		printf("%s", str);
		if(strncmp(str,"ON",2)==0)
		{	
			GPIO_write_pin(22,ON_LED);
			send_string("LED ON\n");
		}
		else
		{
			send_string("LED OFF\n");
			GPIO_write_pin(22,OFF_LED);
		}		
		
	}
	
	
}

