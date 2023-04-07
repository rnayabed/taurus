/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Jul-2020
 Filename		: wifi_demo.c
 Purpose		: wifi Demo
 Description		: wifi_demo Demo with uart interface
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/
/**
 @file wifi_demo.c
 @brief Contains routines for UART interface
 @detail Includes software functions to initialize,
 configure, transmit and receive over UART
 */
/*  Include section
 ***************************************************/
/**
 @file temperature_monitoring_system.c
 @brief Contains routines for demonstrates the temperature monitoring system
 */
#include "uart.h"
#include "stdlib.h"
#include <stdlib.h>
#include "config.h"
#include "gpio.h"
#include "ds18b20.h"
#include <string.h>

#define BUFFER_SIZE 1200
#define AC_ON 	1
#define AC_OFF 	0

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

void receive_string(char * str) {
	char error;
	char data;

	while (1) {
		data = uart_getchar(UART_1, &error);
		//printf("%c",data);
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
	}
}

void at_check() {
	char str[100] = { 0, };
	//printf("AT issued\n");
	send_string("AT\r\n");
	receive_string(str);
	//printf("STR: %s\n",str);
	if (response_ok)
		printf("Modem ready\n");
	else
		printf("error in operation\n");
}

void modem_connect_remote(int id,char * site_name, int port) {
	char str[100] = { 0, };
	char send_str[50]={0,};
	sprintf(send_str,"AT+CIPSTART=%d,\"TCP\",\"%s\",%d\r\n",id, site_name, port);
	printf("STR: %s\n",send_str);
	send_string(send_str);
	receive_string(str);
	printf("STR: %s\n",str);
	if (response_ok)
		printf("Modem connected to remote\n");
	else
		printf("error in operation\n");

}

void modem_GET_remote(int id,char * page,char * domain, char * data) {
	char str[100] = { 0, };
	char send_str[50]={0,};
	char payload[500]={0,};

	sprintf(payload,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n%s",page, domain,data);

	sprintf(send_str,"AT+CIPSEND=%d,%d\r\n",id, strlen(payload));

	printf("STR: %s\n",send_str);
	send_string(send_str);
	receive_string(str);
	printf("STR: %s\n",str);

	printf("STR: %s\n",payload);
	send_string(payload);
	receive_string(str);
	printf("STR: %s\n",str);

	
	if (response_ok)
		printf("Modem GET request send\n");
	else
		printf("error in operation\n");

}

void modem_GET_response() {
	char error;
	char data;

	while (1) {
		data = uart_getchar(UART_1, &error);
		printf("%c",data);
	}
}

void modem_POST_remote(int id,char * page,char * domain, char * data) {
	char str[100] = { 0, };
	char send_str[50]={0,};
	char payload[500]={0,};
/*
POST /path HTTP/1.0\r\n
Content-Type: text/plain\r\n
Content-Length: 12\r\n
\r\n
query_string
*/

	sprintf(payload,"POST %s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s",page, domain,strlen(data),data);

	sprintf(send_str,"AT+CIPSEND=%d,%d\r\n",id, strlen(payload));

	printf("STR: %s\n",send_str);
	send_string(send_str);
	receive_string(str);
	printf("STR: %s\n",str);

	printf("STR: %s\n",payload);
	send_string(payload);
	receive_string(str);
	printf("STR: %s\n",str);

	
	if (response_ok)
		printf("Modem POST request send\n");
	else
		printf("error in operation\n");

}

void modem_POST_response() {
	char error;
	char data;

	while (1) {
		data = uart_getchar(UART_1, &error);
		printf("%c",data);
	}
}



void modem_connect_wifi(char * SSID, char * PASS) {
	char str[100] = { 0, };
	char send_str[50]={0,};
	sprintf(send_str,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS);
	printf("STR: %s\n",send_str);
	send_string(send_str);
	receive_string(str);
	printf("STR: %s\n",str);
	if (response_ok)
		printf("Modem connected to wifi\n");
	else
		printf("error in operation\n");

}

void modem_connection_multi() {
	char str[100] = { 0, };
	send_string("AT+CIPMUX=1\r\n");
	receive_string(str);
	//printf("STR: %s\n",str);
	if (response_ok)
		printf("Modem enabled for multiple connection\n");
	else
		printf("error in operation\n");

}

void modem_ipaddress() {
	char str[100] = { 0, };
	send_string("AT+CIFSR\r\n");
	receive_string(str);
	printf("STR: %s\n", str);
}

void modem_create_server() {
	char str[100] = { 0, };
	send_string("AT+CIPSERVER=1,80\r\n");
	receive_string(str);
	printf("STR: %s\n", str);
	if (response_ok)
		printf("HTTP Server started\n");
	else
		printf("error in operation\n");

}

void modem_disconnect_wifi() {
char str[100] = { 0, };
	send_string("AT+CWQAP\r\n"); //Client mode
	receive_string(str);
	//printf("STR: %s\n",str);
	if (response_ok)
		printf("Wifi disconnected\n");
	else
		printf("error in operation\n");

}



void modem_reset() {
	char str[100] = { 0, };
	send_string("AT+RST\r\n"); 
	receive_string(str);
	printf("STR: %s\n",str);
	if (response_ok)
		printf("Modem reset\n");
	else
		printf("error in operation\n");

}

void modem_client_mode() {
	char str[100] = { 0, };
	send_string("AT+CWMODE=1\r\n"); //Client mode
	receive_string(str);
	//printf("STR: %s\n",str);
	if (response_ok)
		printf("Modem now in client mode\n");
	else
		printf("error in operation\n");

}
int temp;
float celcius, sign;
void read_temperature()
{
		temp = getRawTemperature();
		if (temp == DS18B20_NOT_FOUND)
		{
			printf("DS18B20 Sensor not found at GPIO %d", ONE_WIRE_GPIO);
		}
		else
		{
			sign = 1.0;
			if(temp & TEMP_SIGN_BIT){
				sign = -1.0;
				temp = (temp & (~TEMP_SIGN_BIT));
			}
			celcius = sign * ((float)temp / 16.0);
			printf("Calculated Temp : %f Celcius\n", celcius);
		}
}
extern void ir_send (int toggle);
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
	printf("\n\r INFO: Connect ESP8266 module to UART 1 ");	
	printf("\n\r INFO: Please try to connect the WIFI access point 'ESP-XXXXXX'");	
	printf("\n\r INFO: Switch off the mobile data and try accessing the web page '192.168.4.1'");	
	printf("\n\r *****************************************************************************");
	printf("\n\r ***************************************************************************");
	printf("\n\r INFO: Connect IR sensor to GPIO 4 ");		
	printf("\n\r INFO: Press 1/0 for ON/OFF ");	
	printf("\n\r ***************************************************************************\n\r\n\r");


	printf("\n\r Setting up Wifi Modem \n");
	/*GPIO_write_pin(0,ON);
	udelay(1000);
	GPIO_write_pin(0,OFF);
	udelay(1000);
	GPIO_write_pin(0,ON);*/
	udelay(100000);
	uart_set_baud_rate(UART_1, 115200, 40000000);	
	at_check();
	udelay(1000);	
	modem_client_mode();
	udelay(1000);
	modem_connection_multi();
	udelay(1000);
	modem_ipaddress();
	udelay(1000);
	modem_disconnect_wifi();
	udelay(1000);
	//modem_connect_wifi("vega_wifi","vegawifi");
	modem_connect_wifi("CDAC","");
	printf("\n\rWifi connected\n");
	modem_ipaddress();
	udelay(1000);
	

	//modem_GET_remote(3,"/tms/index.php","10.176.19.212","");
	//modem_GET_response();
	int count=0;
	char send_str[50]={0,};	
	while(1){
		count++;
		read_temperature();
		if(celcius>32){
			ir_send(AC_ON);
			udelay(100000);
			ir_send(AC_ON);
			udelay(100000);
			ir_send(AC_ON);
			udelay(100000);
			sprintf(send_str,"Temp: %2.2f°C, AC: ON",celcius);
		}
		else if(celcius<=20)
		{
			ir_send(AC_OFF);
			udelay(100000);
			ir_send(AC_OFF);
			udelay(100000);
			ir_send(AC_OFF);
			udelay(100000);
			sprintf(send_str,"Temp: %2.2f°C, AC: OFF",celcius);
		}else
		{
			sprintf(send_str,"Temp: %2.2f°C",celcius);
		}
			
		
		modem_connect_remote(3,"10.176.19.212",80);
		modem_POST_remote(3,"/tms/index.php","10.176.19.212",send_str);
		udelay(25000000);
	}
	//modem_POST_response();
	while(1);
}

