/***************************************************************************
* Module name                           :  delay.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  Delay functions fop Buzzer demo.

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

#include "delay.h"
#include "encoding.h"

void delayMicroseconds(unsigned long micro_seconds)
{
#if __riscv_xlen == 64
  unsigned long  cycle_count, current_cycle, target_cycle;
#else
  unsigned int cycle_count, current_cycle, target_cycle;
#endif
  cycle_count = micro_seconds * SYS_FREQ;

  current_cycle = read_csr(mcycle);
  target_cycle = current_cycle + cycle_count;

  while ((read_csr(mcycle) < target_cycle))
    ;
}

void delayMilliseconds(unsigned long  milli_seconds)
{

  unsigned long  cycle_count, current_cycle, target_cycle;

  cycle_count = milli_seconds * SYS_FREQ * 1000;

  current_cycle = read_csr(mcycle);
  target_cycle = current_cycle + cycle_count;

  while ((read_csr(mcycle) < target_cycle))
    ;
}
