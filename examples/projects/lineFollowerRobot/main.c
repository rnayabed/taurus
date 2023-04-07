/***************************************************************************
* Module name                           :  main.c
* Name of Author                        :  Anoop Varghese, C-DAC
* Email ID  (Report any bugs)           :  vanoop@cdac.in
* Module Description                    :  Linefollower robot.

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
#include "gpio.h"
#include "uart.h"
#include "pwmc.h"

#define SYS_CLK_FREQ 40000000UL
#define DISABLE 0
#define ENABLE 1

#define PROXIMITY_LEFT 0
#define PROXIMITY_RIGHT 1

#define LEFT_A 3
#define LEFT_B 4

#define RIGHT_A 5
#define RIGHT_B 6

#define LEFT_EN 7
#define RIGHT_EN 8

void start(void){
	GPIO_write_pin(LEFT_EN,ENABLE);
	GPIO_write_pin(RIGHT_EN,ENABLE);
}
void stop(void){
	GPIO_write_pin(LEFT_EN,DISABLE);
	GPIO_write_pin(RIGHT_EN,DISABLE);
}
void initRobot(void){
	stop();
	GPIO_write_pin(LEFT_A,DISABLE);
	GPIO_write_pin(LEFT_B,DISABLE);
	GPIO_write_pin(RIGHT_A,DISABLE);
	GPIO_write_pin(RIGHT_B,DISABLE);
	start();
}

void moveForward(){
	stop();
	udelay(100);
	GPIO_write_pin(LEFT_A,ENABLE);
	GPIO_write_pin(LEFT_B,DISABLE);
	GPIO_write_pin(RIGHT_A,ENABLE);
	GPIO_write_pin(RIGHT_B,DISABLE);
}

void moveRight(){
	stop();
	udelay(100);
	GPIO_write_pin(LEFT_A,ENABLE);
	GPIO_write_pin(LEFT_B,DISABLE);
	GPIO_write_pin(RIGHT_A,DISABLE);
	GPIO_write_pin(RIGHT_B,ENABLE);
}

void moveLeft(){
	stop();
	udelay(100);
	GPIO_write_pin(LEFT_A,DISABLE);
	GPIO_write_pin(LEFT_B,ENABLE);
	GPIO_write_pin(RIGHT_A,ENABLE);
	GPIO_write_pin(RIGHT_B,DISABLE);
}

void moveHalt(){
	GPIO_write_pin(LEFT_A,DISABLE);
	GPIO_write_pin(LEFT_B,DISABLE);
	GPIO_write_pin(RIGHT_A,DISABLE);
	GPIO_write_pin(RIGHT_B,DISABLE);
}


void main(){
	int leftOK,rightOK;
	printf("+--------[ Line Follower Robot ]-------+\n");
	while(1){
		leftOK = (~GPIO_read_pin(PROXIMITY_LEFT) & 0x1);
		rightOK = (~GPIO_read_pin(PROXIMITY_RIGHT) & 0x1);
		
		if(leftOK && rightOK){
			moveForward();
		}
		else if (leftOK)
		{
			moveRight();
		}else if (rightOK)
		{
			moveLeft();
		}else
		{
			moveHalt();
		}
	}
}