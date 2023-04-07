/***************************************************
 Project Name	: MDP - Microprocessor Development Project
 Project Code	: HD083D
 Created		: 9-Aug-2021
 Filename		: main.c
 Purpose		: I2C BMP180 demo
 Description	: Sample BMP180 with I2C interface
 Author(s)	   	: Thomas Sebastian
 Email			: vega@cdac.in
 
 See LICENSE for license details.
 ***************************************************/

/**
 @file main.c
 @brief contains routines for I2C BMP180 interface
 @detail Includes software functions declarations to initialize,
 configure, write and read BMP180 over I2C interface
 */

#include "SFE_BMP180.h"
#include "stdlib.h"
#include "math.h"

int main()
{
    printf("Demo for BMP180 ");

    begin(); // init

    while (1)
    {
        double T, P;

        startTemperature();
        getTemperature(&T); // init reading temperature
        printf("\n\r Temperature = %f ", T);

        startPressure(0);   // Select mode 0-3
        getPressure(&P, T); // init reading pressure
        printf("\n\r Pressure = %f ", P);

        usleep(1000000);
    }
    return 0;
}
