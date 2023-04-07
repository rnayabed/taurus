#ifndef MAX30100_H
#define MAX30100_H

#include "MAX30100_Registers.h"
    // int MAX30100_begin();
    // void MAX30100_setMode(Mode mode);
    // void MAX30100_setLedsPulseWidth(LEDPulseWidth ledPulseWidth);
    // void MAX30100_setSamplingRate(SamplingRate samplingRate);
    // void MAX30100_setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
    // void MAX30100_setHighresModeEnabled(int enabled);
    // void MAX30100_update();
    // int MAX30100_getRawValues(uint16_t *ir, uint16_t *red);
    // void MAX30100_resetFifo();
    // void MAX30100_startTemperatureSampling();
    // int MAX30100_isTemperatureReady();
    // float MAX30100_retrieveTemperature();
    // void MAX30100_shutdown();
    // void MAX30100_resume();
    uint8_t MAX30100_getPartId();
    uint8_t MAX30100_reset();
    uint8_t MAX30100_init();
    uint8_t MAX30100_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);
    uint8_t readRegister_continuous(uint8_t address, uint8_t length);

    //CircularBuffer<SensorReadout, RINGBUFFER_SIZE> readoutsBuffer;

    uint8_t readRegister(uint8_t address);
    void writeRegister(uint8_t address, uint8_t data);
    void MAX30100_burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length);
    void MAX30100_readFifoData();

#endif
