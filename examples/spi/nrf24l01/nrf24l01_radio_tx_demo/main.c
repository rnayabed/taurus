/***************************************************


 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		   : 16-Aug-2021
 Filename		: main.c
 Purpose		   : spi NRF24L01 demo
 Description	: Sample NRF24L01 radio transceiver with spi interface
 Author(s)		: Thomas Sebastian,CDAC
 Email			: vega@cdac.in

 See LICENSE for license details.
 ***************************************************/

/**
 @brief NRF24L01 demo.
 @details interface NRF24L01 module with  spi .
 @param[in] No input parameter. 
 @param[Out] No output parameter. 
*/

#include "stdlib.h"
#include "spi.h"
#include "nrf24.h"

int main()
{

   uint8_t temp;
   uint8_t q = 0;
   uint8_t data_array[20];
   uint8_t tx_address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
   uint8_t rx_address[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};

   /* simple greeting message */
   printf("\r\n> TX device ready\r\n");

   /* init hardware pins */
   nrf24_init(0);

   /* Channel #2 , payload length: 20 */
   nrf24_config(2, 20);

   /* Set the device addresses */
   nrf24_tx_address(tx_address);
   nrf24_rx_address(rx_address);

   while (1)
   {
      // Fill the data buffer
      /* data_array[0] = 0x00;
        data_array[1] = 0xAA;
        data_array[2] = 0x55;
        data_array[3] = q++;                                 
      */

      char data_string[15] = {"hello world"}; // send string

      // Automatically goes to TX mode
      nrf24_send(data_string);

      // Wait for transmission to end
      while (nrf24_isSending())
         ;

      // Make analysis on last tranmission attempt
      temp = nrf24_lastMessageStatus();

      if (temp == NRF24_TRANSMISSON_OK)
      {
         printf("> Tranmission went OK\r\n");
      }
      else if (temp == NRF24_MESSAGE_LOST)
      {
         printf("> Message is lost ...\r\n");
      }

      // Retranmission count indicates the tranmission quality
      temp = nrf24_retransmissionCount();
      printf("> Retranmission count: %d\r\n", temp);

      // Optionally, go back to RX mode ...
      nrf24_powerUpRx();

      // Or you might want to power down after TX
      // nrf24_powerDown();

      // Wait a little ...
      udelay(600000);

      // RX mode

      if (nrf24_dataReady())
      {
         printf("\r\n> RX device ready\r\n");

         nrf24_getData(data_array);
         printf(">[%2d] ", (int)data_array[0]);
         data_array[0] = ' ';
         printf("%s\r\n", data_array);
      }
   }
}
