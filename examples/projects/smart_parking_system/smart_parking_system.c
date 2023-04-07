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
 @file smart_parking_system.c
 @brief Contains routines for demonstrates the smart parking system
 */
/*  Include section
 ***************************************************/

#include "ssd1306.h"
#include "stdlib.h"
#include "gpio.h"
#include "uart.h"
#include "interrupt.h"
#include "stdlib.h"
#include <stdlib.h>
#include "config.h"

#include <string.h>

#define BUFFER_SIZE 1500

char TXBuffer[BUFFER_SIZE];

#define PARKING_CAR_HEIGHT 23

char http_request[BUFFER_SIZE];
char http_page[BUFFER_SIZE];
char html_page[BUFFER_SIZE];

int response_ok = 0;
int connection_id = 0;
char led_status = 0;

char parking_status = 0;

extern char buffer_logo[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];
extern char carOO[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];
extern char carOX[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];
extern char carXO[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];
extern char carXX[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];

long microsecondsToCentimeters(long microseconds) {
	return microseconds / 29 / 2;
}
/**
 @fn int send_string(char * str) 
 @brief send the string to modem 
 @details 
 @param[in] char * str
 @param[Out] No ouput parameter.
 @return response status

 */
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
/**
 @fn void receive_string(char * str)
 @brief capture the string from modem 
 @details 
 @param[in] char * str
 @param[Out] No ouput parameter.
 @return Void function.

 */
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
/**
 @fn modem_cip_response
 @brief For checking the modem response
 @details 
 @param[in] const char* body
 @param[Out] No ouput parameter.
 @return Void function.

 */
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

const char led_body_no_refresh[] =	"<!DOCTYPE html> <html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"> <title>VEGA Smart Parking System</title> <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; height:100%;} body{margin-top: 50px;background-image: linear-gradient(to top, #a8edea 0%, #fed6e3 100%);} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;} {font-size: 14px;color: #888;margin-bottom: 10px;} .button {display: block;width: 80px;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}.button-on {background-image: linear-gradient(to top, #0ba360 0%, #3cba92 100%);} .button-off { background: linear-gradient(to top, #eb3349, #f45c43);} </style> </head><body><h1>VEGA Processor</h1> <h3>Smart Parking System</h3>";

const char led_body[] = "<!DOCTYPE html> <html><head> <meta http-equiv=\"Refresh\" content=\"0; URL=/\"></head> </html>";


const char led_park_4[] = "<a class=\"button button-on\" >Slot 1</a> <a class=\"button button-on\" >Slot 2</a></body></html>";
const char led_park_3[] = "<a class=\"button button-on\" >Slot 1</a> <a class=\"button button-off\" >Slot 2</a></body></html>";
const char led_park_2[] = "<a class=\"button button-off\" >Slot 1</a> <a class=\"button button-on\" >Slot 2</a></body></html>";
const char led_park_1[] = "<a class=\"button button-off\" >Slot 1</a> <a class=\"button button-off\" >Slot 2</a></body></html>";
const char led_park_0[] = "<a class=\"button button-on\" >Waiting for status</a></body></html>";

const char led_html_end[] = "</body></html>";
/**
 @fn void modem_server_page(const char* body)
 @brief To display the server page
 @details 
 @param[in] const char* body
 @param[Out] No ouput parameter.
 @return Void function.

 */
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
/**
 @fn void modem_server_notfound(const char* body)
 @brief To display the server not found response
 @details 
 @param[in] const char* body
 @param[Out] No ouput parameter.
 @return Void function.

 */
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
/**
 @fn http_connect
 @brief Wifi modem http connect loop
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
void http_connect() {
	char error;

	char data,fdata;
	int i = 0;
	char *token;
	const char s1[2] = ",";
	const char s2[2] = " ";

	//while (1) {
	data = uart_getchar(UART_1, &error);
	fdata = data;
	//printf("%c", data);

	if (data == '+') {
		data = uart_getchar(UART_1, &error);
	}
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

					printf("%d, page: %s\n", connection_id, token);

					response_ok = 0;

					if (strcmp(token, "/") == 0) {
						if (parking_status == 4) {
							strcpy(html_page, led_body_no_refresh);
							strcat(html_page, led_park_4);
							//strcat(html_page, led_html_end);
							modem_server_page(html_page);
						} else if (parking_status == 3) {
							strcpy(html_page, led_body_no_refresh);
							strcat(html_page, led_park_3);
							//strcat(html_page, led_html_end);
							modem_server_page(html_page);
						} else if (parking_status == 2) {
							strcpy(html_page, led_body_no_refresh);
							strcat(html_page, led_park_2);
							//strcat(html_page, led_html_end);
							modem_server_page(html_page);
						} else if (parking_status == 1) {
							strcpy(html_page, led_body_no_refresh);
							strcat(html_page, led_park_1);
							//strcat(html_page, led_html_end);
							modem_server_page(html_page);
						} else {
							strcpy(html_page, led_body_no_refresh);
							strcat(html_page, led_park_0);
							//strcat(html_page, led_html_end);
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
		//printf("# %c", fdata);
	//}

	//}
}

/** @fn uart_intr_handler
 @brief  uart_intr_handler
 @details The function will execute the steps as described in routine
 @warning
 @param[in] unsigned char, unsigned int
 @param[Out] No output parameter.
 */
void uart_intr_handler(void) {

	UC status = 0, error = 0;

	//printf("\nhandler\n");

	status = UartReg(UART_1).UART_IIR_FCR;
//	printf("\nintr_status: %02x\n",status);
	//uart_intr_enable(UART_1,TX_DISABLE,RX_DISABLE);
	http_connect();
	//uart_intr_enable(UART_1,TX_DISABLE,RX_ENABLE);
	return;
}
/**
 @fn at_check
 @brief AT command check in WIFI modem
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
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
/**
 @fn modem_connection_multi
 @brief Multi connection mode enable in WIFI modem
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
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
/**
 @fn modem_ipaddress
 @brief To display the ipaddress of WIFI modem
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
void modem_ipaddress() {
	char str[100] = { 0, };
	send_string("AT+CIFSR\r\n");
	receive_string(str);
	printf("STR: %s\n", str);
}
/**
 @fn modem_create_server
 @brief Server creation command for WIFI modem
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
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
/**
 @fn modem_host_mode
 @brief Host mode command for WIFI modem
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
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

/** @fn uart_register_isr
 @brief  Enable uart interrupt in unmasked & user defined mode..
 @details The selected timer's interrupt is enabled.
 @warning
 @param[in] unsigned char timer_no: Selected timer.
 @param[Out] No output parameter.
 */
void uart_register_isr(UC timer_no, void (*uart_isr)()) { ///*timer_isr is function pointer to user defined intr handler
	interrupt_enable(1);		//Enable interrupt in controller.
	irq_register_handler(1, uart_isr);
}

/**
 @brief Main function for Smart Parking System Demo
 @details Application contains SSD1306 OLED display, Ultrasound sensors & ESP8266 Wifi module
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */
void main() {
	led_status = 0;
	char distance_str[100] = { 0, };
	clock_t total_time = 0;
	UL distance1 = 0, distance2 = 0;
	UI park_prev_status = 0;

	printf("\n\r *****************************************************************************");
	printf("\n\r INFO: Connect SSD1306 OLED module to I2C0 ");
	printf("\n\r INFO: Connect ultrasound sensor 1 [HC-SR04] to GPIO 0 [Trigger]/GPIO 1 [Echo] ");
	printf("\n\r INFO: Connect ultrasound sensor 2 [HC-SR04] to GPIO 2 [Trigger]/GPIO 3 [Echo] ");
	printf("\n\r INFO: Connect ESP8266 module to UART 1 ");
	printf("\n\r INFO: Please try to connect the WIFI access point 'ESP-XXXXXX'");
	printf("\n\r INFO: Switch off the mobile data and try accessing the web page '192.168.4.1'");
	printf("\n\r *****************************************************************************");

	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

	fill_logo(buffer_logo);
	ssd1306_display();
	ssd1306_clearDisplay();
	udelay(50000);

	GPIO_write_pin(22, OFF_LED);
	printf("\n\r Setting up Wifi Modem \n");
	uart_set_baud_rate(UART_1, 115200, 40000000);

	at_check();
	modem_host_mode();
	udelay(1000);
	modem_connection_multi();
	udelay(1000);
	modem_ipaddress();
	udelay(1000);
	modem_create_server();
	printf("\n\rServer running..\n");
	uart_register_isr(UART_1, uart_intr_handler);
	//printf("\n\rISR registered\n");
	uart_intr_enable(UART_1, TX_DISABLE, RX_ENABLE);
	//printf("\n\rINTR Enabled\n");

	//http_connect();
	do {
		//printf("GPIO Section\n");

		GPIO_write_pin(GPIO_0, LOW);
		GPIO_write_pin(GPIO_0, HIGH);
		udelay(10);
		GPIO_write_pin(GPIO_0, LOW);
		uart_intr_enable(UART_1, TX_DISABLE, RX_DISABLE);
		total_time = pulse_duration(GPIO_1, HIGH);

		distance1 = microsecondsToCentimeters(total_time);

		//printf("Distance :%4d cm\n",distance1);

		 GPIO_write_pin(GPIO_2,LOW);
		 GPIO_write_pin(GPIO_2,HIGH);
		 udelay(10);
		 GPIO_write_pin(GPIO_2,LOW);

		 total_time = pulse_duration(GPIO_3,HIGH);

		 distance2 = microsecondsToCentimeters(total_time);


		if (distance1 > PARKING_CAR_HEIGHT && distance2 > PARKING_CAR_HEIGHT) {
			//sprintf(distance_str,"Park Lot 1, 2 Available");
			parking_status = 4;
			if (park_prev_status != parking_status)
				fill_logo(carOO);
		} else if ((distance1 < PARKING_CAR_HEIGHT && distance2 > PARKING_CAR_HEIGHT)) {
			//sprintf(distance_str,"Park Lot 1 : Available");
			parking_status = 3;
			if (park_prev_status != parking_status)
				fill_logo(carXO);
		} else if ((distance1 > PARKING_CAR_HEIGHT && distance2 < PARKING_CAR_HEIGHT)) {
			//sprintf(distance_str,"Park Lot 2 : Available");
			parking_status = 2;
			if (park_prev_status != parking_status)
				fill_logo(carOX);
		} else if ((distance1 < PARKING_CAR_HEIGHT && distance2 < PARKING_CAR_HEIGHT)) {
			//sprintf(distance_str,"Parking Full");
			parking_status = 1;
			if (park_prev_status != parking_status)
				fill_logo(carXX);
		}

		if (park_prev_status != parking_status) {
			ssd1306_display();
			ssd1306_clearDisplay();
			park_prev_status = parking_status;
		}
		//printf("Distance : %4d cm , %4d cm\n",distance1, distance2);
		uart_intr_enable(UART_1, TX_DISABLE, RX_ENABLE);
		udelay(250000);

	} while (1);

}

