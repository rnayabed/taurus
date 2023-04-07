
#include "interrupt.h"

#include "uart.h"
#include "stdlib.h"

#include "config.h"
#include "gpio.h"
#include "ds18b20.h"
#include "esp8266.h"
#include <string.h>

#include "timer.h"

#define UART1_INTR_NUM 1 // Note : this number is for thejas32, number may change on other SoCs
#define BUFFER_SIZE 200

#define AC_ON 	1
#define AC_OFF 0

char ac_status=0;
unsigned char led_toggle=0;
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



/** @fn timer0_intr_handler
  @brief  timer 0 intr handler.
  @details The function will execute the steps as described in routine
  @warning 
  @param[in] unsigned char, unsigned int
  @param[Out] No output parameter.
*/
void ac_control_handler(void) {
	UI wEOI;
	wEOI = Timer(0).EOI;			// Reads the Timer 0 EOI register to clear the intr.
	printf("\n\r Checking AC");
	// User can add their code for TImer 0 interrupt.
	read_temperature();
	led_toggle = ~led_toggle;
	printf("led : %d\n",led_toggle);
	GPIO_write_pin(4,(led_toggle&0x1)); //YELLOW LED
	
		if(celcius>32){
			ir_send(AC_ON);
			udelay(100000);
			ir_send(AC_ON);
			udelay(100000);
			ir_send(AC_ON);
			udelay(100000);
			//sprintf(send_str,"Temp: %2.2f°C, AC: ON",celcius);
			ac_status=1;
			GPIO_write_pin(3,ON_LED); //RED LED
		}
		else if(celcius<=22)
		{
			ir_send(AC_OFF);
			udelay(100000);
			ir_send(AC_OFF);
			udelay(100000);
			ir_send(AC_OFF);
			udelay(100000);
			//sprintf(send_str,"Temp: %2.2f°C, AC: OFF",celcius);
			ac_status=2;
			GPIO_write_pin(3,OFF_LED); //RED LED
		}
	return;   
}


/**
 @fn main
 @brief transmit and reception through uart
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 */
int debug=0;
void main()
{
	
	
	debug=0;
	led_toggle=0;
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

	for(int i=0;i<10;i++)
	{
	  GPIO_write_pin(3,ON_LED); //RED LED
	  udelay(5000);
	  GPIO_write_pin(3,OFF_LED); //RED LED	   
	  GPIO_write_pin(4,ON_LED); //RED LED
          udelay(5000);
 	  GPIO_write_pin(4,OFF_LED); //RED LED
	  GPIO_write_pin(5,ON_LED); //RED LED
          udelay(5000);
	  GPIO_write_pin(5,OFF_LED); //RED LED	 
	}
	 

	
	printf("\n\r Setting up Wifi Modem \n");
	uart_set_baud_rate(UART_1, 115200, 40000000); // initializing uart0
	int count=0;

	

	GPIO_write_pin(5,ON_LED); //GREEN LED

	
	timer_run_in_intr_mode(TIMER_0,0x3FFFFFFF);          // Load timer2 and enable interrupt.

	
	timer_register_isr(TIMER_0, ac_control_handler);  // Register timer2 Intr.
	timer_disable(TIMER_0);

	while(1){
		char ip_address[4]={0,};
		char response[200]={0,};
		char send_str[50]={0,};
		esp8266_restart();
		printf("AT+RST->OK\r\n");
		esp8266_isStarted();
		printf("AT->OK\r\n");
		esp8266_mode(1);
		printf("AT+CWMODE=1->OK\r\n");
		esp8266_disconnect();
		printf("Wi-Fi disconnected\r\n");
		esp8266_connect("CDAC","");
		printf("Wi-Fi connected\r\n");
		esp8266_ip(ip_address);
		printf("IP address: %d.%d.%d.%d\r\n",ip_address[0],ip_address[1],ip_address[2],ip_address[3]);
	
		if(ip_address[0]!=0){			
			read_temperature();
			sprintf(send_str,"Temp: %2.2f°C",celcius);
			if(ac_status==1){			
				strcat(send_str, ", AC: ON");				
			}
			else if(ac_status==2)
			{			
				strcat(send_str, ", AC: OFF");
			}	
			ac_status = 0;
			
			esp8266_start(ESP8266_TCP,"10.176.18.49",80);
			printf("CIPSTART ->OK\r\n");		
			esp8266_send("/tms/index.php","10.176.18.49",send_str);
			printf("CIPSEND ->OK\r\n");
		}
		else
			printf("No IP address, retrying..\r\n");

		timer_enable(TIMER_0);		
		udelay(25000000);
		timer_disable(TIMER_0);		
	}
	
}
