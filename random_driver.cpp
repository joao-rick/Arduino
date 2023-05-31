/**
   @file
   @author Lucas Câmara <lucascamara10123@hotmail.com>
   @version 1.0

   Driver para valores aleatórios (.cpp)
*/

#include <Arduino.h>
#include "random_driver.h"

random_driver::random_driver (): Sensor (1) {
}

bool random_driver::sense (float value[]) {
 	value[0] = data[0];
	return true;
}

void random_driver::update()
{
  data[0] =  random(0,1023);
}

void random_driver::reset()
{
  data[0] = 0;
}
