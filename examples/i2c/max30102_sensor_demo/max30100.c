#include "i2c_api.h"
#include "MAX30100_Registers.h"
#include "MAX30100.h"

#define TRUE 1
#define FALSE 0

#define DEFAULT_MODE                MAX30100_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE       MAX30100_SAMPRATE_100HZ
#define DEFAULT_PULSE_WIDTH         MAX30100_SPC_PW_1600US_16BITS
#define DEFAULT_RED_LED_CURRENT     MAX30100_LED_CURR_50MA
#define DEFAULT_IR_LED_CURRENT      MAX30100_LED_CURR_50MA
#define EXPECTED_PART_ID            0x15
#define RINGBUFFER_SIZE             16

int MAX30100_begin()
{    

   if (MAX30100_getPartId() != EXPECTED_PART_ID) {
	printf("PART ID WRONG, %02x\n",MAX30100_getPartId());
        return FALSE;
    }
//     printf("setMode\n");
//     MAX30100_setMode(DEFAULT_MODE);
//     printf("setLedsPulseWidth\n");
//     MAX30100_setLedsPulseWidth(DEFAULT_PULSE_WIDTH);
//     printf("setSamplingRate\n");
//     MAX30100_setSamplingRate(DEFAULT_SAMPLING_RATE);
//     printf("setLedsCurrent\n");
//     MAX30100_setLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
//     printf("setHighresModeEnabled\n");
//     MAX30100_setHighresModeEnabled(TRUE);

    return TRUE;
}

// void MAX30100_setMode(Mode mode)
// {
//     writeRegister(MAX30100_REG_MODE_CONFIGURATION, mode);
//     printf("mode_: %04x\n",mode);
// 	 uint8_t mode_ = readRegister(MAX30100_REG_MODE_CONFIGURATION);
//     printf("mode_: %04x\n",mode_);
// }

// void MAX30100_setLedsPulseWidth(LEDPulseWidth ledPulseWidth)
// {
//     uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
//     printf("spo2 conf: %04x\n",previous);
   
//     writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);

//      previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
//     printf("spo2 conf: %04x\n",previous);
// }

// void MAX30100_setSamplingRate(SamplingRate samplingRate)
// {
//     uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
//     writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
// }

// void MAX30100_setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
// {
//     writeRegister(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
// }

// void MAX30100_setHighresModeEnabled(int enabled)
// {
//     uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
//     if (enabled) {
//         writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
//     } else {
//         writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
//     }
// }

uint8_t MAX30100_reset() 
{
		i2c_init(I2C_1); //System clock =40MHz and I2C clock =100 kHz
    writeRegister(REG_MODE_CONFIG, 0x40);
	return 1;
}

uint8_t readRegister(uint8_t address)
{
    i2c_beginTransmission(MAX30100_I2C_ADDRESS);
    i2c_write(address);
    i2c_endTransmission(TRUE);
    i2c_requestFrom(MAX30100_I2C_ADDRESS, 1, TRUE);
		// udelay(100);
    return i2c_read();
}

uint8_t readRegister_continuous(uint8_t address, uint8_t length)
{		
    i2c_beginTransmission(MAX30100_I2C_ADDRESS);
    i2c_write(address);
		i2c_endTransmission(TRUE);
    i2c_requestFrom(MAX30100_I2C_ADDRESS, length, TRUE);
		// udelay(100);
		return TRUE;
}

void writeRegister(uint8_t address, uint8_t data)
{
     i2c_beginTransmission(MAX30100_I2C_ADDRESS);
     i2c_write(address);
     i2c_write(data);
     i2c_endTransmission(TRUE);
  // udelay(100);
}

uint8_t MAX30100_getPartId()
{
    return readRegister(0xff);
}

uint8_t MAX30100_init()
{
    writeRegister(REG_INTR_ENABLE_1,0xc0);
    writeRegister(REG_INTR_ENABLE_2,0x00);
    writeRegister(REG_FIFO_WR_PTR,0x00);
    writeRegister(REG_OVF_COUNTER,0x00);
    writeRegister(REG_FIFO_RD_PTR,0x00);
    writeRegister(REG_FIFO_CONFIG,0x4f);
    writeRegister(REG_MODE_CONFIG,0x03);
    writeRegister(REG_SPO2_CONFIG,0x27);
    writeRegister(REG_LED1_PA,0x24);
    writeRegister(REG_LED2_PA,0x24);
    writeRegister(REG_PILOT_PA,0x7f);
    return TRUE;
}

uint8_t MAX30100_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
  uint32_t un_temp;
  uint8_t uch_temp;
  *pun_ir_led=0;
  *pun_red_led=0;

	uint8_t rd_ptr = 0;

	 //printf("\nInside Read FIFO\n");

  uch_temp = readRegister(REG_INTR_STATUS_1);
	// printf("Completed Reading Interrupt Status 1 Register");
  uch_temp = readRegister(REG_INTR_STATUS_2);
	// printf("Completed Reading Interrupt Status 2 Register");
  if (readRegister_continuous(REG_FIFO_DATA,6))
	{
		// rd_ptr = readRegister(REG_FIFO_RD_PTR);
		// printf("\nRead Pointer 1 = %d" , rd_ptr);
		 
		un_temp = i2c_read();
		un_temp<<=16;
		*pun_red_led += un_temp;
		 //rd_ptr = readRegister(REG_FIFO_RD_PTR);
		 //printf("\nRead Pointer 2 = %d" , rd_ptr);
		 
		un_temp=i2c_read();
		un_temp<<=8;
		*pun_red_led+=un_temp;
		// rd_ptr = readRegister(REG_FIFO_RD_PTR);
		// printf("\nRead Pointer 2 = %d" , rd_ptr);
		 
		un_temp=i2c_read();
		*pun_red_led+=un_temp;
		// rd_ptr = readRegister(REG_FIFO_RD_PTR);
		// printf("\nRead Pointer 2 = %d" , rd_ptr);
		 
		un_temp=i2c_read();
		un_temp<<=16;
		*pun_ir_led+=un_temp;
		// rd_ptr = readRegister(REG_FIFO_RD_PTR);
		// printf("\nRead Pointer 2 = %d" , rd_ptr);
		 
		un_temp=i2c_read();
		un_temp<<=8;
		*pun_ir_led+=un_temp;
		// rd_ptr = readRegister(REG_FIFO_RD_PTR);
		// printf("\nRead Pointer 2 = %d" , rd_ptr);
		 
		un_temp=i2c_read();
		*pun_ir_led+=un_temp;
	}
  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
  return TRUE;
}
