/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 21-april-2021
 Filename		: voice_recg_demo.c
 Purpose		: interfacing voice recognition module with vega board
 Description	: for voice assistance
 Author(s)		: thomas sebastian
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @file voice_recg_demo.c
 @brief contains routines for uart voice_recg_module  interface
 @detail Includes software functions declarations to initialize,
 configure, write and read voice_recg_module over uart interface
 */

#include "uart.h"
#include "stdlib.h"
#include "config.h"
#include "gpio.h"
#include "led.h"
#include "string.h"

// index number for loading the pretrained voice data with resp index number.

#define VOICE_ON 0x0
#define VOICE_OFF 0x01
#define VOICE_START 0x02
#define VOICE_BLINK 0x03
#define VOICE_HELLO 0x04
#define VOICE_dummy_1 0x05
#define VOICE_dummy_2 0x06
#define VOICE_STOP 0x07

#define START_DATA 0xAA
#define END_DATA 0x0A

#define MAX_OUT_LEN 15
#define LENGTH_INDEX 3
#define STRING_INDEX 4
#define ID_INDEX 1
#define LED_GREEN 22
#define LED_RED 24
#define LED_BLUE 23

int voiceUart, count;
char error;
char data;
int response_ok = 0;

void sig_train(unsigned char index, int length, char *buf)
{
    int outputRemining = 7;
    char outputData[20];

    uart_putchar(voiceUart, 0xAA, &error); //start package
    uart_putchar(voiceUart, 0x03 + length, &error);
    uart_putchar(voiceUart, 0x21, &error);  //cmd
    uart_putchar(voiceUart, index, &error);
    for (size_t i = 0; i < length; i++)
    {
        uart_putchar(voiceUart, buf[i], &error);
    }
    uart_putchar(voiceUart, 0x0A, &error); // end package

    count = 1;
    outputData[outputRemining] = uart_getchar(voiceUart, &error);

    while (count != 0)
    {
        outputData[outputRemining] = uart_getchar(voiceUart, &error);

        // printf("%x : %c \n", outputData[outputRemining], outputData[outputRemining]);
        // printf("%x  \n", outputData[outputRemining]);
        //outputRemining--;

        if (outputData[outputRemining] != 0x0)
        {
            printf("%c", outputData[outputRemining]);
            //printf("%x : %c \n", outputData[outputRemining], outputData[outputRemining]);
            count++;
        }
        else
        {
            outputData[outputRemining] = uart_getchar(voiceUart, &error);
            // printf("%x : %c \n", outputData[outputRemining], outputData[outputRemining]);
            return;
            if (outputData[outputRemining] == END_DATA)
            {
                return;
            }
        }
    }
}
void enableIndex(unsigned char index)
{
    int outputRemining = 7;
    char outputData[8];

    uart_putchar(voiceUart, 0xAA, &error); // start package
    uart_putchar(voiceUart, 0x03, &error);
    uart_putchar(voiceUart, 0x30, &error);  // cmd
    uart_putchar(voiceUart, index, &error);
    uart_putchar(voiceUart, 0x0A, &error);  // end package
    while (outputRemining)
    {
        outputData[outputRemining] = uart_getchar(voiceUart, &error);
        //printf("%x \n", outputData[outputRemining]);
        outputRemining--;
    }
    if (outputData != 0)
    {
        printf("\n Voice index 0x%x activated.\n", index);
    }
    else
    {
        printf("\n Voice index 0x%x failed to activate.\n", index);
    }
}
int voiceRecog()
{
    int indx, length;
    char recogData[MAX_OUT_LEN];

    //printf("\n voiceRecog ");
    /* data = uart_getchar(UART_1, &error);*/
    indx = 0;
    recogData[indx] = uart_getchar(voiceUart, &error); /* wait for start data */
    if (recogData[indx] != START_DATA)
    {
        return -1;
    }
    while (uart_getchar(voiceUart, &error) != 0xFF)
        ;
    while (indx < MAX_OUT_LEN) /* read rest data */
    {
        indx++;
        recogData[indx] = uart_getchar(voiceUart, &error);
        if (recogData[indx] == END_DATA)
        {
            break;
        }
    }
    length = recogData[LENGTH_INDEX];
    indx = STRING_INDEX;
    printf("\n Command[%d], len = %02d : ", recogData[ID_INDEX], length);
    while (length)
    {
        printf("%c", recogData[indx]);
        indx++;
        length--;
    }
    return (recogData[ID_INDEX]);
}

void main()
{
    int cmd, num;
    char signatue[20], tokn;
    voiceUart = 1;
    uart_set_baud_rate(voiceUart, 9600, SYSTEM_FREQUENCY);

    printf("+---------------[ voice_recg_demo ]--------------------------+\n\n");
    printf("+                                       		         +\n\n");
    printf("+ 	    	connect RXD pin to IO1	           	      	 +\n\n");
    printf("+	       	connect TXD pin to IO0	                   	 +\n\n");
    printf("+		                                               	 +\n\n");
    printf("+------------------------------------------------------------+\n\n");

    while (1)
    {
        printf("+	enter 1 --> Training   <<<>>>  2 --> Loading & recognition      +\n\n");

        printf("\n enter a number:\n");
        tokn = uart_getchar(UART_0, &error);

        if (tokn == '1')
        {
            /*
            @ this is for training a new command to the system.
            @ use the fn 'sig_train' with the index number given as VOICE_HELLO.
            @ you can use VOICE_dummy_1 && VOICE_dummy_2 for training , as they are not used yet.
            @ can overwrite a command by using the same index number ,for training another command.
            */

            strcpy(signatue, "Hello");           // change signature "Hello" according to your command.
            sig_train(VOICE_HELLO, 5, signatue); // replace VOICE_HELLO with your resp index number.
        }
        else if (tokn == '2')
        {

            /*
            @ this is for Loading the trained commands to the system.
            @ use the fn 'enableIndex'  with the index numbers as given  below.
            @ enable the index numbers according to your pgm.
            */
            enableIndex(VOICE_HELLO);
            enableIndex(VOICE_ON);
            enableIndex(VOICE_BLINK);
            enableIndex(VOICE_STOP);
            // enableIndex(VOICE_OFF);
            // enableIndex(VOICE_START);

            while (1)
            {

                cmd = voiceRecog(); //  enable voice recognition
                switch (cmd)
                {
                case VOICE_ON:
                    on_LED(LED_BLUE);
                    off_LED(LED_RED);
                    break;
                case VOICE_STOP:
                    on_LED(LED_RED);
                    udelay(10000);
                    off_LED(LED_RED);
                    off_LED(LED_GREEN);
                    off_LED(LED_BLUE);
                    break;
                case VOICE_BLINK:
                    off_LED(LED_RED);
                    off_LED(LED_GREEN);
                    for (size_t i = 0; i < 5; i++)
                    {
                        on_LED(LED_BLUE);
                        udelay(5000);
                        off_LED(LED_BLUE);
                        udelay(5000);
                    }
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            printf("\n enter a valid number \n\n");

        }
    }
}