#ifndef __ds3231_h_
#define __ds3231_h_

/**
 @file ds3231_rtc.h
 @brief header file for ds3231_rtc 
 @detail 
 */

#include "i2c.h"



// i2c slave address of the DS3231 chip
#define DS3231_I2C_ADDR_WR             0xD0
#define DS3231_I2C_ADDR_RD           0xD1

// timekeeping registers
#define DS3231_TIME_CAL_ADDR        0x00
#define DS3231_ALARM1_ADDR          0x07
#define DS3231_ALARM2_ADDR          0x0B
#define DS3231_CONTROL_ADDR         0x0E
#define DS3231_STATUS_ADDR          0x0F
#define DS3231_AGING_OFFSET_ADDR    0x10
#define DS3231_TEMPERATURE_ADDR     0x11

// control register bits
#define DS3231_CONTROL_A1IE     0x1		/* Alarm 2 Interrupt Enable */
#define DS3231_CONTROL_A2IE     0x2		/* Alarm 2 Interrupt Enable */
#define DS3231_CONTROL_INTCN    0x4		/* Interrupt Control */
#define DS3231_CONTROL_RS1	    0x8		/* square-wave rate select 2 */
#define DS3231_CONTROL_RS2    	0x10	/* square-wave rate select 2 */
#define DS3231_CONTROL_CONV    	0x20	/* Convert Temperature */
#define DS3231_CONTROL_BBSQW    0x40	/* Battery-Backed Square-Wave Enable */
#define DS3231_CONTROL_EOSC	    0x80	/* not Enable Oscillator, 0 equal on */


// status register bits
#define DS3231_STATUS_A1F      0x01		/* Alarm 1 Flag */
#define DS3231_STATUS_A2F      0x02		/* Alarm 2 Flag */
#define DS3231_STATUS_BUSY     0x04		/* device is busy executing TCXO */
#define DS3231_STATUS_EN32KHZ  0x08		/* Enable 32KHz Output  */
#define DS3231_STATUS_OSF      0x80		/* Oscillator Stop Flag */


typedef struct{
    UC sec;         /* seconds */
    UC min;         /* minutes */
    UC hour;        /* hours */
    UC wday;        /* day of the week */
    UC mday;        /* day of the month */
    UC mon;         /* month */
    UC year_s;      /* year in short notation*/

    UC yday;        /* day in the year */
    UC isdst;       /* daylight saving time */


    UI year;        /* year */
#ifdef CONFIG_UNIXTIME
    uint32_t unixtime;   /* seconds since 01.01.1970 00:00:00 UTC*/
#endif
}RTC_TIME;

void i2c_get_rtc_DS3231(UC i2c_num, UC Slave_Address_Wr,
		UC Slave_Address_Rd, UC RTC_Address);
void i2c_set_rtc_DS3231(UC i2c_num, UC RTC_Address, UC Slave_Address_Wr);




UC BcdToDec( UC val );
UC DecToBcd(UC val);

#endif
