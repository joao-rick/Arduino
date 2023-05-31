/**
	@file DHT22_driver.cpp
	@author Saulo Daniel Ferreira Pontes
	@version 1.1 07/2015

	Grove: Humidity Temperature Sensor pro
*/

#include "DHT22_driver.h"

DHT22_Sensor::DHT22_Sensor() : Sensor(2)
{
}

bool DHT22_Sensor::sense (float value[]) 
{
 value[0] = data[0];
 value[1] = data[1];

 return true;
}

void DHT22_Sensor::pin(int pin)
{
 PIN_0 = pin;

 dht = new DHT(PIN_0, DHT22);

 dht->begin();
}

void DHT22_Sensor::update()
{
 data[0] = dht->readTemperature();
 data[1] = dht->readHumidity();
}

void DHT22_Sensor::reset()
{
  data[0] = 0;
  data[1] = 0;
}
