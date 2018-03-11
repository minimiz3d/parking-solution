/**
 * @brief Código que implementa os métodos responsáveis pela percepção do veículo com base nos sensores de refletância.
 * 
 * @file Follow.cpp
 * @author Leonardo Schmidt
 * @date 2018-03-10
 */

#include "Follow.h"

/**
 * @brief Construtor do objeto "Follow".
 * 
 * @param sensor Define qual dos sensores de refletância está sendo instanciado.
 */
Follow::Follow(int sensor){
	this->sensor = sensor;
}

/**
 * @brief Método que inicializa o sensor em questão como dado de entrada.
 * 
 */
void Follow::setup(){
	pinMode(this->sensor, INPUT);
}

/**
 * @brief 
 * 
 * @return int Valor lido pelo sensor.
 */
int Follow::read(){
	return analogRead(this->sensor);
}

/**
 * @brief Método que verifica se a leitura ultrapassou o valor limite de refletância esperado.
 * 
 * @param treshold Valor limite de refletância.
 * @param sensor Identificação do sensor.
 * @return true Quando a refletância lida for superior ao limite estabelecido - há uma marcação no chão.
 * @return false Quando o valor lido estiver dentro do esperado.
 */
bool Follow::alert(int treshold, int sensor){
	if(read() > treshold){
		return true;
	}
	return false;
}

/**
 * @brief Método que verifica se a marcação no chão é preta.
 * 
 * @author Árthur Tolfo Pinheiro
 * @param notBlack Valor associado à cor preta.
 * @return true Quando não há tarja preta no chão.
 * @return false Quando há tarja preta no chão.
 */
bool Follow::checkLL(int notBlack) {
	if (read() < notBlack) {
		return true;
	} 
	return false;
}