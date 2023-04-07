/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: -mar-2021
 Filename		: vegabrd_firebase_demo.c
 Purpose		: vegabrd_firebase_demo
 Description		: nodemcu with uart_1 interface
 Author(s)	   	: thomas sebastian
 Email			: vega@cdac.in
 
 See LICENSE for license details.
 ***************************************************/
/**
 @file vegabrd_firebase_demo.c
 @brief Contains uart communication with nodemcu with vega board
 @ burn this code to vega board and execute.
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
	//printf("rec Func : ");	
  while(1){
	 data = uart_getchar(UART_1, &error);
	//printf("%x",data);	
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
 @details string is transmitted and received through uart

 */
void main() {

	char error;       
	char str[100]={0,};
	
	
	uart_set_baud_rate(UART_1,9600,SYSTEM_FREQUENCY); //setting baudrate
	
	printf("\n\r Setting up the Device \n\r");
	send_string("H \n");
 while(1){

		//memset(str,0,sizeof(str));
		
	receive_string(str);	// getting Rx 
	printf("%s", str);
		if(strncmp(str,"Start",2)==0)             // string comparison with the Rx signal from nodemcu
		{	
			send_string("HELLO WORLD \n");  //Tx
			//udelay(5000);
		}
		else if(strncmp(str,"Next",2)==0)        // string comparison with the Rx signal from nodemcu
		{
			send_string("123.4 \n");       //Tx
			//udelay(5000);
		}
		else
		{
			send_string("error \n");
			//udelay(5000);
		}
	
	
	printf("\n\r done !! \n\r");  // end msg
	}
	
	
}

