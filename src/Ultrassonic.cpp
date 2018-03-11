/**
 * @brief Código que implementa funções do sensor ultrassônico.
 * 
 * @file Ultrassonic.cpp
 * @author Leonardo Schimdt
 * @date 2018-03-10
 */

#include "Ultrassonic.h"

/**
 * @brief Construtor vazio do objeto "Ultrassonic."
 * 
 */
Ultrassonic::Ultrassonic() {}

/**
 * @brief Construct a new Ultrassonic:: Ultrassonic object
 * 
 * @param trigPin Pino de "trigger" do módulo ultrassônico.
 * @param echoPin Pino emissor do módulo.
 */
Ultrassonic::Ultrassonic(int trigPin, int echoPin){
	this->trigPin = trigPin;
	this->echoPin = echoPin;
}

/**
 * @brief Método que inicializa os pinos do módulo como saída e entrada.
 * 
 */
void Ultrassonic::setup() {
	pinMode(this->trigPin, OUTPUT);
	pinMode(this->echoPin, INPUT);
}

/**
 * @brief Método que obtém a distância do veículo em relação a algum obstáculo.
 * 
 * @return long Distância em relação ao que se deseja medir. No caso desta aplicação é uma parede.
 */
long Ultrassonic::getDistance() {
	long duration, distance;
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distance = (duration / 2) / 29.1;
	return (distance);
}

