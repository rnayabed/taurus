/***************************************************
 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 08-Jul-2020
 Filename		: gps_demo.c
 Purpose		: GPS sensor demo
 Description		: transmission & reception with uart interface
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in

 See LICENSE for license details.
 ***************************************************/
/**
 @file gps_demo.c
 @brief Contains routines for demonstrates the gps sensor communication
 */
 
/*  Include section
 ***************************************************/
#include "uart.h"
#include "stdlib.h"
#include <string.h>

char GPRMC_data[300] = { 0, };
char * values[300];
//char GPRMC_data[300]="GPRMC,061655.00,A,0830.56694,N,07657.71509,E,2.821,,090920,,,A*70";
char * utc_time;
char * navig_status;
char * lat;
char * lat_direction;
char * lon;
char * lon_direction;
char * sog;
char * azimuth;
char * utc_date;
char * utc_date;

void dateParse(char * date_str) {
	printf("DATE \t\t: %c%c/%c%c/%c%c\n", date_str[0], date_str[1], date_str[2], date_str[3], date_str[4], date_str[5]);
}

void timeParse(char * time_str) {
	printf("UTC TIME \t: %c%c:%c%c:%c%c\n", time_str[0], time_str[1], time_str[2], time_str[3], time_str[4], time_str[5]);
}

/*GPRMC,061655.00,A,0830.56694,N,07657.71509,E,2.821,,090920,,,A*70*/
/*--------------------------------------------------------------------------\
 |            | NMEA          | Decimal                                     ||
 |------------|---------------|-----------------------------------------------
 | latitude   | 2511.120738   | 25 + (11.120738/60) = 25.185345633          ||
 |------------|---------------|-----------------------------------------------
 | longitude  | 05516.820909  | 055 + (16.820909/60) = 55.280348483         ||
 \--------------------------------------------------------------------------*/
#define MAX_LONGITUDE    180
#define MAX_LATITUDE     90

#define LAT 1
#define LON 2

/**
 * Convert latitude,longitude from nmea to decimal
 * @param type lat = 1, lon = 2
 */
double degToDecimal(char *nmea, char type, unsigned char *dir) {
	int idx, dot = 0;
	double dec = 0;
	for (idx = 0; idx < strlen(nmea); idx++) {
		if (nmea[idx] == '.') {
			dot = idx;
			break;
		}
	}

	if (dot < 3)
		return 0;

	int i, dd;
	double mm;
	char cdd[5], cmm[10];
	memset(&cdd, 0, 5);
	memset(&cmm, 0, 10);
	strncpy(cdd, nmea, dot - 2);
	strcpy(cmm, nmea + dot - 2);
	dd = atoi(cdd);
	mm = atof(cmm);

	dec = dd + (mm / 60);

	if (type == 1 && dec > MAX_LATITUDE)
		return 0;
	else if (type == 2 && dec > MAX_LONGITUDE)
		return 0;

	if (strcmp(dir, "N") == 0 || strcmp(dir, "E") == 0)
		return dec;
	else
		return -1 * dec;
}

/**
 * Splits a string by comma.
 *
 * string is the string to split, will be manipulated. Needs to be
 *        null-terminated.
 * values is a char pointer array that will be filled with pointers to the
 *        splitted values in the string.
 * max_values is the maximum number of values to be parsed.
 *
 * Returns the number of values found in string.
 */
static int split_string_by_comma(char *string, char **values, int max_values)
{
	int i = 0;

	values[i++] = string;
	while (i < max_values && NULL != (string = strchr(string, ','))) {
		*string = '\0';
		values[i++] = ++string;
	}

	return i;
}

void getGPSData() {

	char error;
	char data;
	int i = 0;
	char *token;
	const char s1[2] = ",";

	while (1) {
		memset(GPRMC_data, 0, i);
		data = uart_getchar(UART_1, &error);
		//printf("%c", data);

		if (data == '$') {
			data = uart_getchar(UART_1, &error);
			if (data == 'G') {
				data = uart_getchar(UART_1, &error);
				if (data == 'P') {
					data = uart_getchar(UART_1, &error);
					if (data == 'R') {
						GPRMC_data[i] = data;
						while (data != '\n') {
							i++;
							data = uart_getchar(UART_1, &error);
							GPRMC_data[i] = data;
						}
						i++;
						GPRMC_data[i] = '\0';

						//printf("%s\n", GPRMC_data);
						split_string_by_comma(GPRMC_data,values,10);

						/*for (int k = 0; k < 10; k++)
							printf("%s\n", values[k]);*/
						i = 0;
						break;
				   }
				}
			}

		}
	}
}

/**
 @fn main
 @brief transmit and reception through uart
 @details 1 character is transmitted and received through uart
 @param[in] No input parameter.
 @param[Out] No ouput parameter.
 @return Void function.

 */

/*
 *
 $GPRMC,061655.00,A,0830.56694,N,07657.71509,E,2.821,,090920,,,A*70
 $GPVTG,,T,,M,2.821,N,5.225,K,A*2A
 $GPGGA,061655.00,0830.56694,N,07657.71509,E,1,04,3.03,3.1,M,-95.1,M,,*71
 $GPGSA,A,2,15,13,29,18,,,,,,,,,3.19,3.03,1.00*0D
 $GPGSV,2,1,05,13,25,052,31,15,59,062,31,18,25,336,31,23,,,21*49
 $GPGSV,2,2,05,29,73,013,15*45
 $GPGLL,0830.56694,N,07657.71509,E,061655.00,A,A*62

 */
void main() {
	int wait=1;
	char error;
	char str[100] = { 0, };
	unsigned char data = 'A';

	printf("\n\r *****************************************************************************");
	printf("\n\r INFO: Connect GPS6MV2 module to UART 1 ");		
	printf("\n\r *****************************************************************************");


	uart_set_baud_rate(UART_1, 9600, 40000000);
	{
		getGPSData();

		/*for (int k = 0; k < 10; k++)
			printf(">> %s\n", values[k]);*/
		printf("\n=============================================\n");
		if(strlen(values[1])>5)
			timeParse(values[1]);

		//printf("utc_date \t: %s\n",utc_date);
		if(strlen(values[1])>5)
			dateParse(values[9]);

		if (strcmp(values[2], "A") == 0) {
			//printf("utc_time \t: %s\n",utc_time);
			//printf("getGPSData over\n");

			//printf("navig_status \t: %s\n",navig_status);
			//printf("latitude \t: %s\n",lat);
			//printf("lat_direction \t: %s\n",lat_direction);
			double latitude = degToDecimal(values[3], LAT, values[4]);
			printf("latitude \t: %f\n", latitude);
			//printf("longitude \t: %s\n",lon);
			//printf("lon_direction \t: %s\n",lon_direction);
			double longitude = degToDecimal(values[5], LON, values[6]);
			printf("longitude \t: %f\n", longitude);

			//printf("speed over ground \t: %s\n",values[7]);
			//printf("azimuth \t: %s\n",values[8]);
			printf("MAP : http://maps.google.co.uk/maps?f=q&hl=en&q=%f,%f\n", latitude, longitude);
			while(1);
		} else {
			printf("Waiting for Valid GPS data...\n");
		}
		printf("\n=============================================\n");
	}

	while(1);

}

