/**
	@file DHT22_driver.h
	@author Saulo Daniel Ferreira Pontes
	@version 1.1 07/2015

	Grove: Humidity Temperature Sensor pro
*/

#ifndef DHT22_DRIVER_H_
#define DHT22_DRIVER_H_

#include <Arduino.h>
#include "device.h"
#include "DHT.h"

class DHT22_Sensor : public Sensor 
{
private:
       int PIN_0;
       float data[2]; //[0]:umidity, [1]:temperature;
       DHT *dht;

public:
	DHT22_Sensor();
	bool sense (float[]);
        void pin(int);
        void reset();
        void update();
};

#endif /* DHT22_DRIVER_H_ */
