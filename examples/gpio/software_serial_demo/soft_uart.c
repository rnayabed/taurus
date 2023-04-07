#include "stdlib.h"
#include "gpio.h"
#include "soft_uart.h"
#include "encoding.h"

volatile US *gpioDirAddr1 = (volatile US *)(GPIO_0_BASE_ADDRESS + 0x40000); // GPIO 0 direction register.
volatile US *gpioDirAddr2 = (volatile US *)(GPIO_1_BASE_ADDRESS + 0x40000); // GPIO 1 direction register.

unsigned char TX;
unsigned char RX;

#define read_const_csr(reg) ({ unsigned long __tmp; \
  asm ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

__attribute__ ((noinline))
void tunedDelay(unsigned long delay){
    unsigned long start_mcycle = read_csr(mcycle);
    do { } while (((read_csr(mcycle) - start_mcycle)*0.025) < delay);
}  

/* Sets GPIO pin in input mode.*/
void setWireInput(unsigned char gpioPin)
{
	unsigned short bitPos, dirData;

	if (gpioPin > 15)
	{
		bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
		dirData = *gpioDirAddr2;		// Address of the direction register.
		dirData &= ~(bitPos);			// Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr2 = dirData;		// Data written to direction register.
		__asm__ __volatile__("fence");
	}
	else
	{
		bitPos = (1 << gpioPin); // Align the selected pin to its position.
		dirData = *gpioDirAddr1; // Address of the direction register.
		dirData &= ~(bitPos);	 // Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr1 = dirData; // Data written to direction register.
		__asm__ __volatile__("fence");
	}
}

/* Sets GPIO pin in output mode.*/
void setWireOutput(unsigned char gpioPin)
{
	unsigned short bitPos, dirData;

	if (gpioPin > 15)
	{
		bitPos = (1 << (gpioPin - 16)); // Align the selected pin to its position.
		dirData = *gpioDirAddr2;		// Address of the direction register.
		dirData |= (bitPos);			// Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr2 = dirData;		// Data written to direction register.
		__asm__ __volatile__("fence");
	}
	else
	{
		bitPos = (1 << gpioPin); // Align the selected pin to its position.
		dirData = *gpioDirAddr1; // Address of the direction register.
		dirData |= (bitPos);	 // Clearing a bit configures the pin to be INPUT.
		*gpioDirAddr1 = dirData; // Data written to direction register.
		__asm__ __volatile__("fence");
	}
}

/* Sets GPIO pin in output mode.*/
void setGpioPin(unsigned char gpioPin, unsigned char data)
{
	unsigned short bitPos, *gpioDirAddrPtr;
	unsigned long gpioDirAddrData;

	if (gpioPin > 15)
	{
		gpioPin = gpioPin - 16;
		bitPos = (1 << (gpioPin)); // Align the selected pin to its position.
		gpioDirAddrData = GPIO_1_BASE_ADDRESS;
	}
	else
	{
		bitPos = (1 << gpioPin); // Align the selected pin to its position.
		gpioDirAddrData = GPIO_0_BASE_ADDRESS;
	}

	gpioDirAddrData |= (bitPos << 2); // Generate address of the selected GPIO pin in GPIO 0.
	gpioDirAddrPtr = (US *)gpioDirAddrData;
	*gpioDirAddrPtr = (data << gpioPin);
	//printf("%016lx : %016lx \n",gpioDirAddrPtr,(data << gpioPin));
	__asm__ __volatile__("fence");
}


US getGpioPin(US pin_no) {

	UC gpio_number = 0;
	US bit_position = 0;

	UL gen_gpio_0_addr,gen_gpio_1_addr;
	volatile US *gpio_0_data, *gpio_1_data;
	US read_data = 0;

	if(pin_no <= 15)
		gpio_number = 0; 			// Pins 0 to 15 defined in GPIO 0.
	else	
		gpio_number = 1;			// Pins 16 to 31 defined in GPIO 1.


	if(gpio_number == 0)
	{
		bit_position = (1 << pin_no);		// Align the selected pin to its position.
		gen_gpio_0_addr = GPIO_0_BASE_ADDRESS;	// Base address of GPIO 0.
		gen_gpio_0_addr+=(bit_position << 2);	// Generate address of the selected GPIO pin in GPIO 0.
		gpio_0_data = (US *)gen_gpio_0_addr;	// Generated address.
		read_data = *gpio_0_data; 		// Read data from the address.
		
	}
	else if(gpio_number == 1)
	{
		pin_no = (pin_no - 16);
		bit_position = (1 << pin_no);		// Align the selected pin to its position.
		gen_gpio_1_addr = GPIO_1_BASE_ADDRESS;	// Base address of GPIO 1.
		gen_gpio_1_addr+=(bit_position << 2);	// Generate address of the selected GPIO pin in GPIO 1.
		gpio_1_data = (US *)gen_gpio_1_addr;	// Generated address.
		read_data = *gpio_1_data;		// Read data from the address.
	}
	__asm__ __volatile__ ("fence");

	if(read_data)
		return 1; 				// GPIO pin is High.
	else
		return 0; 				// GPIO pin is Low.
}


int soft_uart1(unsigned char tx, unsigned char rx) {
	//Set TX pin as gpio output
	setWireOutput(tx);
	setGpioPin(tx,HIGH);	
	//Set TX pin as gpio input
	setWireInput(rx);	
	TX =tx;
	RX =rx;
	return 0;
}
//soft_uart_getchar 9600 BAUD
char soft_uart_getchar() {
char byte=0;
	while (1) {	
		while(getGpioPin(RX)); //start bit
		tunedDelay(45);
		if(getGpioPin(RX)!=0)
			continue;
		for (unsigned char i=0; i<8;i++) //Parse bits
		{
			tunedDelay(85);
			byte |= getGpioPin(RX)<<i;
		}
		tunedDelay(80);
		if(getGpioPin(RX)!=1)// detect stop bit
		 { 
			printf("Error : No stop bit %02x",byte);
		 }
		else
		  break;

	}
	return byte;
}
//soft_uart_putchar 9600 BAUD
int soft_uart_putchar(char byte) {
	
	setGpioPin(TX,LOW);
	tunedDelay(100);
	for (unsigned char mask = 0x01; mask; mask <<= 1)
	{
		if (byte & mask) // choose bit
		   setGpioPin(TX,HIGH); // send 1
		else
		   setGpioPin(TX,LOW); // send 0

		tunedDelay(85);
	}
	setGpioPin(TX,HIGH); // restore pin to natural state
	tunedDelay(1000);
	return 0;
}

int soft_uart_send(char * str) {//send a string to soft uart
	while(*str!='\0')
	{
		soft_uart_putchar((char)*str);
		str++;
	}
	return 0;
}

void soft_uart_receive(char * str) {
	char data;
	while (1) {
		data = soft_uart_getchar();
		*str = data;
		if(data=='\0') //exepecting an end of string, please change accordingly for break the loop
		{
			break;
		}
		str++;		
	}
}

