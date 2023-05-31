/**
	@file
	@author Felipe Tavares <felipe.oltavares@gmail.com>
	@version 1.0

	Biblioteca para descrever dispositivos (.h)
*/

#ifndef SBOARD_DEVICE_H
#define SBOARD_DEVICE_H

/** Descreve um dispositivo atuador
	
	Atuadores transformam um vetor real em
	uma variação no ambiente.
	Esta classe deve ser extendida para implementar
	qualquer dispositivo real.
*/
class Actuator {
	//! Graus de liberdade
	unsigned int degree;
public:
	/** Construtor
	
		@param degree Número de graus de liberdade do atuador
	*/
	Actuator (unsigned int);

	/** Ativa o atuador
	
		@param value O valor a ser convertido em ação
		@return true se o atuador foi ativado, false se houve algum problema
	*/
	virtual bool act(float[]) = 0;
	
	/** Número de graus do atuador

		@returns número de graus do atuador
	*/
	unsigned int getDegree();
};

/** Descreve um dispositivo sensor
	
	Sensores transformam uma série variações no ambiente
	em um vetor real.
	Esta classe deve ser extendida para implementar
	qualquer dispositivo real.
*/
class Sensor {
	//! Graus de liberdade
	unsigned int degree;
public:
	/** Construtor
	
		@param degree Número de graus de liberdade do sensor
	*/
	Sensor (unsigned int);

	/** Lê o sensor
	
		@param value O valor lido.
		@return true se o sensor foi lido, false se houve algum problema
	*/
	virtual bool sense(float[]) = 0;

	/** Número de graus do sensor

		@returns número de graus do sensor
	*/
	unsigned int getDegree();
};

#endif /* SBOARD_DEVICE_H */
