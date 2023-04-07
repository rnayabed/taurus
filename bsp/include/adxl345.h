#ifndef _ADXL345_H
#define _ADXL345_H
/**
 @file adxl345.h
 @brief header file for ADXL345 sensor 
 @detail 
 */
/***************************************************
* Module name: ADXL345.h
*
* Copyright 2020 Company CDAC(T).
* All Rights Reserved.
*
*  The information contained herein is confidential
* property of Company. The user, copying, transfer or
* disclosure of such information is prohibited except
* by express written agreement with Company.
*
*
* Module Description:
* ADXL345 registers and function declarations
*
***************************************************/
/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define ADXL345_I2C_ADDR_WR	(0xA6) ///< Assumes ALT address pin low
#define ADXL345_I2C_ADDR_RD	(0xA7) ///< Assumes ALT address pin low
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
#define ADXL345_REG_DEVID (0x00)        ///< Device ID
#define ADXL345_REG_THRESH_TAP (0x1D)   ///< Tap threshold
#define ADXL345_REG_OFSX (0x1E)         ///< X-axis offset
#define ADXL345_REG_OFSY (0x1F)         ///< Y-axis offset
#define ADXL345_REG_OFSZ (0x20)         ///< Z-axis offset
#define ADXL345_REG_DUR (0x21)          ///< Tap duration
#define ADXL345_REG_LATENT (0x22)       ///< Tap latency
#define ADXL345_REG_WINDOW (0x23)       ///< Tap window
#define ADXL345_REG_THRESH_ACT (0x24)   ///< Activity threshold
#define ADXL345_REG_THRESH_INACT (0x25) ///< Inactivity threshold
#define ADXL345_REG_TIME_INACT (0x26)   ///< Inactivity time
#define ADXL345_REG_ACT_INACT_CTL                                              \
  (0x27) ///< Axis enable control for activity and inactivity detection
#define ADXL345_REG_THRESH_FF (0x28) ///< Free-fall threshold
#define ADXL345_REG_TIME_FF (0x29)   ///< Free-fall time
#define ADXL345_REG_TAP_AXES (0x2A)  ///< Axis control for single/double tap
#define ADXL345_REG_ACT_TAP_STATUS (0x2B) ///< Source for single/double tap
#define ADXL345_REG_BW_RATE (0x2C)        ///< Data rate and power mode control
#define ADXL345_REG_POWER_CTL (0x2D)      ///< Power-saving features control
#define ADXL345_REG_INT_ENABLE (0x2E)     ///< Interrupt enable control
#define ADXL345_REG_INT_MAP (0x2F)        ///< Interrupt mapping control
#define ADXL345_REG_INT_SOURCE (0x30)     ///< Source of interrupts
#define ADXL345_REG_DATA_FORMAT (0x31)    ///< Data format control
#define ADXL345_REG_DATAX0 (0x32)         ///< X-axis data 0
#define ADXL345_REG_DATAX1 (0x33)         ///< X-axis data 1
#define ADXL345_REG_DATAY0 (0x34)         ///< Y-axis data 0
#define ADXL345_REG_DATAY1 (0x35)         ///< Y-axis data 1
#define ADXL345_REG_DATAZ0 (0x36)         ///< Z-axis data 0
#define ADXL345_REG_DATAZ1 (0x37)         ///< Z-axis data 1
#define ADXL345_REG_FIFO_CTL (0x38)       ///< FIFO control
#define ADXL345_REG_FIFO_STATUS (0x39)    ///< FIFO status
/*=========================================================================*/

/* Constants

***************************************************/
#define ADXL345_MG2G_MULTIPLIER (0.004) ///< 4mg per lsb
#define SENSORS_GRAVITY_STANDARD (9.80665F)              /**< Earth's gravity in m/s^2 */


/**
 * @brief  Used with register 0x31 (ADXL345_REG_DATA_FORMAT) to set g range
 *
 */
typedef enum {
  ADXL345_RANGE_16_G = 0b11, ///< +/- 16g
  ADXL345_RANGE_8_G = 0b10,  ///< +/- 8g
  ADXL345_RANGE_4_G = 0b01,  ///< +/- 4g
  ADXL345_RANGE_2_G = 0b00   ///< +/- 2g (default value)
} range_t;

/*  Define section
* 
*
***************************************************/






/***************************************************/
 

/*  Function declaration section
* 
*
***************************************************/
US i2c_ADXL345_get_acc(UC i2c_num, UC Slave_Address_Wr,
		UC Slave_Address_Rd, UC Adxl_Address);
void i2c_ADXL345_set_range(UC i2c_num, UC Slave_Address_Wr,range_t range,UC Adxl_Address);
void i2c_ADXL345_write_register(UC i2c_num, UC Slave_Address_Wr,UC Adxl_Address,UC value);
UC i2c_ADXL345_read_register(UC i2c_num, UC Slave_Address_Wr,UC Slave_Address_Rd,UC Adxl_Address);

#endif	/* _ADXL345_H */



