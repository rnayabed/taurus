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
 @brief Contains routines for ESP8266 wifi module demonstration
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
	}
}

void modem_cip_response() {
	char error;
	char data;

	while (1) {
		data = uart_getchar(UART_1, &error);

		//Check for OK
		if (data == 'O') {
			data = uart_getchar(UART_1, &error);

			if (data == 'K') {
				response_ok = 1;
				break;
			}
		}
		//Check for ERROR
		if (data == 'E') {
			data = uart_getchar(UART_1, &error);

			if (data == 'R') {
				data = uart_getchar(UART_1, &error);

				if (data == 'R') {
					response_ok = 0;
					break;
				}
			}
		}
	}
}

const char led_body_no_refresh[] =
		"<!DOCTYPE html> <html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"> \
  <title>LED Control</title> \
  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} \
  body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;} \
  {font-size: 14px;color: #888;margin-bottom: 10px;} \
  .button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;} \
.button-on {background-color: #1abc9c;} \
.button-on:active {background-color: #16a085;} \
.button-off {background-color: #34495e;} \
.button-off:active {background-color: #2c3e50;}</style> </head>\
<body><h1>VEGA Processor</h1> \
<h3>ESP8266 Demo</h3> ";

const char led_body[] =
		"<!DOCTYPE html> <html><head> <meta http-equiv=\"Refresh\" content=\"0; URL=/\"></head> </html>";

const char led_on_1[] = "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>";
const char led_off_1[] = "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>";
const char led_html_end[] = "</body></html>";

void modem_server_page(const char* body) {
	char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: ";

	char contentLength[16];
	sprintf(contentLength, "%d\r\n\r\n", strlen(body));

	sprintf((char*) TXBuffer, "AT+CIPSEND=%d,%d\r\n", connection_id, strlen(body) + strlen(contentLength) + strlen(header));
	//printf("CDAC STRING: %s",TXBuffer);
	send_string((char*) TXBuffer);

	modem_cip_response();

	if (response_ok)
		printf("CIPSEND OK\n");

	//udelay(100);

	send_string(header);
	//printf("CDAC header: %s\n",header);
	send_string(contentLength);
	//printf("CDAC contentLength: %s\n",contentLength);
	send_string((char*) body);
	//printf("CDAC body: %s\n",body);
}

void modem_server_notfound(const char* body) {
	char header[] = "HTTP/1.1 404 OK\r\nContent-Type: text/plain\r\nConnection: close\r\nContent-Length: ";

	char contentLength[16];
	sprintf(contentLength, "%d\r\n\r\n", strlen(body));

	sprintf((char*) TXBuffer, "AT+CIPSEND=%d,%d\r\n", connection_id, strlen(body) + strlen(contentLength) + strlen(header));
	//printf("CDAC STRING: %s",TXBuffer);
	send_string((char*) TXBuffer);

	modem_cip_response();

	if (response_ok)
		printf("CIPSEND OK\n");

	//udelay(100);

	send_string(header);
	//printf("CDAC header: %s\n",header);
	send_string(contentLength);
	//printf("CDAC contentLength: %s\n",contentLength);
	send_string((char*) body);
	//printf("CDAC body: %s\n",body);
}

void http_connect() {
	char error;
	char data;
	int i = 0;
	char *token;
	const char s1[2] = ",";
	const char s2[2] = " ";

	while (1) {
		data = uart_getchar(UART_1, &error);
		//printf("%c", data);

		if (data == '+') {
			data = uart_getchar(UART_1, &error);
			if (data == 'I') {
				data = uart_getchar(UART_1, &error);
				if (data == 'P') {
					data = uart_getchar(UART_1, &error);
					if (data == 'D') {
						http_request[i] = data;
						while (data != '\n') {
							i++;
							data = uart_getchar(UART_1, &error);
							http_request[i] = data;
						}
						i++;
						http_request[i] = '\0';
						strcpy(http_page, http_request);
						/*for (int k = 0; k < i; k++)
							printf("%c", http_request[k]);*/
						/* get the first token */
						token = strtok(http_request, s1);
						token = strtok(NULL, s1);

						//printf("conn: %s\n", token);
						connection_id = atoi(token);
						token = strtok(http_page, s2);
						token = strtok(NULL, s2);

						printf("%d, page: %s\n",connection_id, token);

						response_ok = 0;
						if (strcmp(token, "/ledon") == 0) {
							strcpy(html_page, led_body);
							GPIO_write_pin(22, ON_LED);
							modem_server_page(html_page);
							led_status = 1;

						} else if (strcmp(token, "/ledoff") == 0) {
							strcpy(html_page, led_body);
							GPIO_write_pin(22, OFF_LED);
							modem_server_page(html_page);
							led_status = 0;
						} else if (strcmp(token, "/") == 0) {
							if (led_status == 0) {
								strcpy(html_page, led_body_no_refresh);
								strcat(html_page, led_on_1);
								strcat(html_page, led_html_end);
								modem_server_page(html_page);
							} else {
								strcpy(html_page, led_body_no_refresh);
								strcat(html_page, led_off_1);
								strcat(html_page, led_html_end);
								modem_server_page(html_page);
							}

						} else {
							modem_server_notfound("Not found");
						}
						memset(http_request, 0, i);
						i = 0;
					}
				}
			}
		}
	}
}

void at_check() {
	char str[100] = { 0, };
	send_string("AT\r\n");
	receive_string(str);
	//printf("STR: %s\n",str);
	if (response_ok)
		printf("Modem ready\n");
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

void modem_host_mode() {
	char str[100] = { 0, };
	send_string("AT+CWMODE=2\r\n");
	receive_string(str);
	//printf("STR: %s\n",str);
	if (response_ok)
		printf("Modem now in host mode\n");
	else
		printf("error in operation\n");

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
	printf("\n\r INFO: Connect ESP8266 module to UART 1 ");	
	printf("\n\r INFO: Please try to connect the WIFI access point 'ESP-XXXXXX'");	
	printf("\n\r INFO: Switch off the mobile data and try accessing the web page '192.168.4.1'");	
	printf("\n\r *****************************************************************************");

	GPIO_write_pin(22, OFF_LED);
	printf("\n\r Setting up Wifi Modem \n");
	uart_set_baud_rate(UART_1, 115200, SYSTEM_FREQUENCY);
	at_check();
	modem_host_mode();
	udelay(1000);
	modem_connection_multi();
	udelay(1000);
	modem_ipaddress();
	udelay(1000);
	modem_create_server();
	printf("\n\rServer running..\n");
	http_connect();

}

