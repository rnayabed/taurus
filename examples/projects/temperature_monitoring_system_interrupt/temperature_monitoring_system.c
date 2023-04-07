
#include "interrupt.h"

#include "uart.h"
#include "stdlib.h"
#include <stdlib.h>
#include "config.h"
#include "gpio.h"
#include "ds18b20.h"
#include <string.h>


#define UART1_INTR_NUM 1 // Note : this number is for thejas32, number may change on other SoCs
#define BUFFER_SIZE 100

#define AC_ON 	1
#define AC_OFF 	0

 char TXBuffer[BUFFER_SIZE];
 int tx_buffer_index;

 char cmd[BUFFER_SIZE]={0,},resp[BUFFER_SIZE]={0,};
 char resp_str[BUFFER_SIZE]={0,};



void uart_intr_handler(void) // interrupt handler
{
	unsigned char status, error, data;	//
	//printf("#");	
	data = uart_getchar(UART_1, &error);   // reading data
	TXBuffer[tx_buffer_index++]=data;
	TXBuffer[tx_buffer_index]='\0';
	status = UartReg(UART_1).UART_IIR_FCR; // clearing interrupt
	return;
}

int send_string(char * str) {
	char error;

	tx_buffer_index=0;	
	while (*str != '\0') {
		uart_putchar(UART_1, *str, &error);		
		str++;
	}
	return 0;
}


void strip_str(char * src, char * dest)
{
	char j=0;
	for(char i=0;i<BUFFER_SIZE;i++)
	{
		if(src[i]=='\n' || src[i]=='\r')
		{
			dest[j++]=' ';
		}else
			dest[j++]=src[i];
	}
	dest[j]='\0';
}

void show_response()
{
	char buffer[BUFFER_SIZE]={0,};
	strip_str(TXBuffer,buffer);
	printf(" %s\n",buffer);
}

int check_response()
{
	char * p=NULL;	
	int resp_len = strlen(resp);
	char buffer[BUFFER_SIZE]={0,};
	for(int i=0;i<50000;i++)
	{		
		if(strlen(TXBuffer)>=resp_len){
			strip_str(TXBuffer,buffer);
			p = strstr(buffer,resp);		
			if(p){		
			  printf(" %s\n",buffer);
			  return 1;
			}
		}			
	}	
	 printf(" [ERROR] Expected : %s, Got : %s \n",resp, buffer);
	return 0;
}

void modem_connect_remote(int id,char * site_name, int port) {	
	char send_str[50]={0,};
	sprintf(send_str,"AT+CIPSTART=%d,\"TCP\",\"%s\",%d\r\n",id, site_name, port);
	strcpy(resp,"OK");	
	send_string(send_str);
	udelay(30000);	
	check_response();
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

	//printf("STR: %s\n",send_str);
	strcpy(resp,">");
	send_string(send_str);
	udelay(30000);	
	check_response();

	//printf("STR: %s\n",payload);
	strcpy(resp,"OK");
	send_string(payload);
	udelay(30000);	
	check_response();
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
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 */
void main()
{
	char send_str[50]={0,};
	

	printf("\n\r +------------------[ Temperature Monitor & Control System ]-----------------+\n", UART_1);
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

	uart_set_baud_rate(UART_1, 115200, 40000000); // initializing uart0
	interrupt_enable(UART1_INTR_NUM); //Enable interrupt in controller.
	irq_register_handler(UART1_INTR_NUM, uart_intr_handler);
	uart_intr_enable(UART_1, TX_DISABLE, RX_ENABLE); // enabling uart0 interrupt

	strcpy(cmd,"AT\r\n");
	strcpy(resp,"OK");
	send_string(cmd);
	udelay(30000);	
	check_response();

	strcpy(cmd,"AT+CWMODE=1\r\n");
	strcpy(resp,"OK");
	send_string(cmd);
	udelay(30000);	
	check_response();


	strcpy(cmd,"AT+CIPMUX=1\r\n");
	strcpy(resp,"OK");
	send_string(cmd);
	udelay(30000);	
	check_response();


	strcpy(cmd,"AT+CIFSR\r\n");
	send_string(cmd);
	udelay(30000);	
	show_response();


	strcpy(cmd,"AT+CWQAP\r\n");
	strcpy(resp,"OK");
	send_string(cmd);
	udelay(30000);	
	check_response();

	sprintf(send_str,"AT+CWJAP=\"%s\",\"%s\"\r\n","CDAC","");	
	strcpy(cmd,send_str);
	send_string(cmd);
	udelay(300000);	
	show_response();

	int count=0;
	//char send_str[50]={0,};	
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
		else if(celcius<=22)
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
	

		while(1);
}
