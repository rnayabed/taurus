#ifndef DHT11_H_
#define DHT11_H_

#include "stdlib.h"
#include "config.h"


/* In case of porting this library,make sure that all delays are accurate.*/

#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0
#define ONE_WIRE_GPIO 1			// Max 0 - 15 in Thejas SoC.



/* Functions for Thejas32 1-wire GPIO */
void setWireInput(void);
void setWireOutput(void);
void writeWire(US bit);
US readWire(void);



void GetDataDHT11();
void dht11Start(void);
void dht11Response(void);
int startTxnDHT();
void GetHumidityDHT11(void);
void GetTemperatureDHT11(void);
void CheckSumDHT11(void);

#endif
