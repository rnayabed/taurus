/***************************************************************************
* Module name                           :  software_interrupt.c
* Name of Author                        :  Sreenadh S, Premjith A V
* Email ID  (Report any bugs)           :  premjith@cdac.in
* Module Description                    :  uart working on interrupt mode.

  Copyright (C) 2020  CDAC(T). All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/


#include "stdlib.h"
#include "config.h"
#include "interrupt.h"


void software_intr_handler(void) // interrupt handler
{			
	printf("MACHINE SW INTR HANDLER\n\r");	
	return;
}

/**
 @fn main
 @brief 
 @details 
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 */
void main()
{
	
	printf("+------[ Software interrupt demo ]------+\n");


	sw_irq_register_handler(software_intr_handler); //Register user defined handler function
	enable_sw_irq(); //Enable interrupt in controller.
	generate_sw_irq();//generates a single sw intr
	disable_sw_irq();//disable when sw intr is no more needed
	while(1);

}
