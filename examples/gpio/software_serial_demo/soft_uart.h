#ifndef SOFT_UART_H_
#define SOFT_UART_H_

#include "stdlib.h"
#include "config.h"

/* Functions for Thejas32 1-wire GPIO */
void setWireInput(unsigned char gpioPin);
void setWireOutput(unsigned char gpioPin);
void setGpioPin(unsigned char gpioPin, unsigned char data);
int soft_uart1(unsigned char tx, unsigned char rx);
int soft_uart_putchar(char byte);
char soft_uart_getchar();
int soft_uart_send( char * str);
void soft_uart_receive(char * str);

#endif /*SOFT_UART_H_*/
