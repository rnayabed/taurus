/***************************************************************************
* Module name                           :  buzzer_demo.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  Buzzer demo program

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
#include "gpio.h"
#include "uart.h"
#include "delay.h"

/*

  The song data used here is adopted from original work of Bhaskar Tripathi.

*/

#define BUZZER_GPIO PIN_0

int length = 149;                                                                                                                                                       // The number of notes
char notes[] = "cdeeeeeeeeedef eeedddbdc ccgggggggggaf fffffeddf eedeedeggaff eeeeeddbdc cdeeeedf efgggfedfe feeeddbdc ccggggeggggga fffffedfe egc bab aga ccddeedef "; // A space represents a rest
int beats[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,
               2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
               2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
               2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
               2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1,
               2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
               1, 1, 1, 1, 2, 2, 2, 1, 1,
               1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,
               2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
               1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
               2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
               1, 1, 4, 1, 1, 1, 4, 1, 1, 1, 4, 1,
               1, 1, 1, 1, 1, 1, 1, 1, 4, 1};
// Set tempo
int tempo = 200;

/* Created a square wave for given time duration */
void playTone(int tone, int duration)
{
  for (long i = 0; i < duration * 1000L; i += tone * 2)
  {
    GPIO_write_pin(BUZZER_GPIO, HIGH);
    delayMicroseconds(tone);
    GPIO_write_pin(BUZZER_GPIO, LOW);
    delayMicroseconds(tone);
  }
}

/* Assigning high time for the notes */
void playNote(char note, int duration)
{
  char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
  int tones[] = {956, 851, 758, 716, 638, 568, 1014, 478};

  // Play the tone corresponding to the note name
  for (int i = 0; i < 8; i++)
  {
    if (names[i] == note)
    {
      playTone(tones[i], duration);
    }
  }
}

void main()
{
  // For the length of the tune array
  for (int i = 0; i < length; i++)
  {
    // Take space as rest
    if (notes[i] == ' ')
    {
      // Rest
      delayMilliseconds(beats[i] * tempo);
    }
    else
    {
      // Play the corresponding note for the corresponding beat
      playNote(notes[i], beats[i] * tempo);
    }

    // Pause between notes
    delayMilliseconds(tempo / 2);
  }

  while (1)
  {
  }
}
