#ifndef ADS1015_H_
#define ADS1015_H_

#define ADS1015_REG_CONFIG_OS_SINGLE (0x8000)       // Write: Set to start a single-conversion

#define ADS1015_REG_POINTER_CONFIG (0x01)           //Configuration
#define ADS1015_REG_POINTER_CONVERT (0x00)          // Conversion

#define ADS1015_REG_CONFIG_PGA_4_096V (0x0200)      // +/-4.096V range = Gain 1

#define ADS1015_REG_CONFIG_MUX_SINGLE_0 (0x4000)    // Single-ended AIN0
#define ADS1015_REG_CONFIG_MUX_SINGLE_1 (0x5000)    // Single-ended AIN1
#define ADS1015_REG_CONFIG_MUX_SINGLE_2 (0x6000)    // Single-ended AIN2
#define ADS1015_REG_CONFIG_MUX_SINGLE_3 (0x7000)    // Single-ended AIN3

#define ADS1015_REG_CONFIG_CQUE_NONE  (0x0003)      // Disable the comparator and put ALERT/RDY in high state (default)
#define ADS1015_REG_CONFIG_CLAT_NONLAT (0x0000)     // Non-latching comparator (default)
#define ADS1015_REG_CONFIG_CPOL_ACTVLOW (0x0000)    // ALERT/RDY pin is low when active (default)
#define ADS1015_REG_CONFIG_CMODE_TRAD (0x0000)      // Traditional comparator with hysteresis (default)
#define ADS1015_REG_CONFIG_DR_1600SPS (0x0080)      // 1600 samples per second (default)
#define ADS1015_REG_CONFIG_DR_3300SPS (0x00C0)      // 3300 samples per second
#define ADS1015_REG_CONFIG_MODE_SINGLE (0x0100)     // Power-down single-shot mode (default)

#define BIT_SHIFT 4
#define ADS1015_CONVERSIONDELAY (1)
#define ADS1015_ADDRESS (0x48)

void ads1015Begin(unsigned char i2c_num);
uint16_t ads1015ReadSingleEnded(uint8_t channel);

#endif