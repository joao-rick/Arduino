/**
	@file
	@author Felipe Tavares <felipe.oltavares@gmail.com>
	@version 1.0

	Biblioteca para descrever dispositivos (.cpp)
*/

#include "device.h"

Actuator::Actuator (unsigned int degree) {
	//! Número de graus = número passado por parâmetro
	this->degree = degree;
}

unsigned int Actuator::getDegree () {
	return degree;
}

Sensor::Sensor (unsigned int degree) {
	//! Número de graus = número passado por parâmetro
	this->degree = degree;	
}

unsigned int Sensor::getDegree () {
	return degree;
}
