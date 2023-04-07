/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Jul-2020
 Filename		: gsm_iot_post_demo.c
 Purpose		: wifi Demo
 Description		: wifi_demo Demo with uart interface
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/
/**
 @file gsm_iot_post_demo.c
 @brief Contains routines for demonstrates the gsm module SMS/Call applications
 */
/*  Include section
 ***************************************************/

#include "uart.h"
#include "stdlib.h"
#include <stdlib.h>
#include "config.h"
#include "gpio.h"
#include <string.h>

#define BUFFER_SIZE 1200

char TXBuffer[BUFFER_SIZE];

char http_request[BUFFER_SIZE];
char http_page[BUFFER_SIZE];
char html_page[BUFFER_SIZE];

int response_ok = 0;
int connection_id = 0;
char led_status = 0;

int send_string(char * str) {
	char error;
	response_ok = 0;
	while (*str != '\0') {
		uart_putchar(UART_1, *str, &error);
		/*if (*str == '\n')
		 uart_putchar(UART_1, '\r', &error);*/
		str++;
	}
	return 0;
}

int send_char(char code) {
	char error;
	uart_putchar(UART_1, code, &error);
	return 0;
}

void receive_string(char * str) {
	char error;
	char data;

	while (1) {
		data = uart_getchar(UART_1, &error);
		*str = data;
		str++;
		//Check for OK
		if (data == 'O') {
			data = uart_getchar(UART_1, &error);
			*str = data;
			str++;
			if (data == 'K') {
				response_ok = 1;
				break;
			}
		}
		//Check for ERROR
		if (data == 'E') {
			data = uart_getchar(UART_1, &error);
			*str = data;
			str++;
			if (data == 'R') {
				data = uart_getchar(UART_1, &error);
				*str = data;
				str++;
				if (data == 'R') {
					response_ok = 0;
					break;
				}
			}
		}
		if (data == 'D') {
			data = uart_getchar(UART_1, &error);
			*str = data;
			str++;
			if (data == 'O') {
				data = uart_getchar(UART_1, &error);
				*str = data;
				str++;
				if (data == 'W') {
					response_ok = 0;
					break;
				}
			}
		}

		if (data == '>') {			
					response_ok = 0;
					break;			
		}
	}
}

int gsm_send(char * sstr) {
	char response[100] = { 0, };	
	send_string(sstr);
	receive_string(response);
	printf("%s\n",response);

	if (response_ok)
		return 1;
	else
		return 0;
}

int gsm_send_data(char * data)
{
	char response[100] = { 0, };
	char send_str[50]={0,};
	sprintf(send_str,"AT+HTTPDATA=%d,100000\r\n", strlen(data));	

	send_string(send_str);
	receive_string(response);
	udelay(500000);
	send_string(data);

	receive_string(response);
	printf("%s\n",response);

	if (response_ok)
		return 1;
	else
		return 0;
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
	led_status = 0;

	printf("\n\r *****************************************************************************");
	printf("\n\r INFO: Connect GSM module [SIM800A] to UART 1 ");
	printf("\n\r *****************************************************************************");


	printf("\n\r Setting up GSM Modem \n");
	uart_set_baud_rate(UART_1, 9600, SYSTEM_FREQUENCY);
	
	udelay(10000);
	gsm_send("AT\n");
	udelay(1000);
	gsm_send("AT+CREG?\r\n"); //check network registration
	udelay(1000);

	gsm_send("AT+CIMI\r\n"); //IMEI display
	udelay(1000);

	gsm_send("ATD+91xxxxxxxxxxxx;\r\n"); //call to mobile - replace it with a vaild mobile number
	udelay(1000);

	gsm_send("AT+CMGF=1\r\n"); //SMS mode
	udelay(1000);

	gsm_send("AT+CSCS=\"GSM\"\r\n"); //Type GSM
	udelay(1000);
	
	gsm_send("AT+CMGS=\"+91xxxxxxxxxxxx\"\r\n"); //Set mobile number -replace it with a vaild mobile number
	udelay(10000);
	
	gsm_send("Hello world\r\n"); //SMS content
	udelay(1000);
	send_char(0x1a); //Ctrl+Z	
	
	
	while(1);
}

