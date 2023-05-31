/**
   @file
   @author Lucas Câmara <lucascamara10123@hotmail.com>
   @version 1.0

   Driver para valores aleatórios (.h)
*/

#include "device.h"

class random_driver: public Sensor {
  float data[1];
public:
       random_driver();

       bool sense (float[]);
       void reset();
       void update();
};
