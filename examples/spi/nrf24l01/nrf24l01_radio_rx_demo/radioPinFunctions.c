/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform spesific functions in this file ...
* -----------------------------------------------------------------------------
*/
#include "stdlib.h"
#include "gpio.h"

/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state)
    {
        GPIO_write_pin(0,HIGH);
        //set_bit
    }
    else
    {   
        GPIO_write_pin(0,LOW);
        //clr_bit
    }
}
/* ------------------------------------------------------------------------- */
