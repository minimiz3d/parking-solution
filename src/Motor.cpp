/**
 * @brief Código que implementa
 * 
 * @file Motor.cpp
 * @author Leonardo Schimdt
 * @date 2018-03-10
 */
#include "Motor.h"

/**
 * @brief Construtor do objeto "Motor".
 * 
 * @param IN1 Entrada 1.
 * @param IN2 Entrada 2.
 * @param IN3 Entrada 3.
 * @param IN4 Entrada 4.
 * @param ENA Pino enable A.
 * @param ENB Pino enable B.
 */
Motor::Motor(int IN1,int IN2,int IN3,int IN4,int ENA,int ENB){
	this->IN1 = IN1;
	this->IN2 = IN2;
	this->IN3 = IN3;
	this->IN4 = IN4;
	this->ENA = ENA;
	this->ENB = ENB;
}

/**
 * @brief Método de inicialização dos pinos de saída.
 * 
 */
void Motor::setup(){
	pinMode(this->IN1, OUTPUT);
	pinMode(this->IN2, OUTPUT);
	pinMode(this->IN3, OUTPUT);
	pinMode(this->IN4, OUTPUT);
	pinMode(this->ENA, OUTPUT);
	pinMode(this->ENB, OUTPUT);
}

/**
 * @brief Método que define a direção "em frente" para o veículo.
 * 
 */
void Motor::forward(){
	analogWrite(this->ENA, this->speed);
	analogWrite(this->ENB, this->speed);
	digitalWrite(this->IN1, LOW);
	digitalWrite(this->IN3, LOW);
	digitalWrite(this->IN2, HIGH);
	digitalWrite(this->IN4, HIGH);
}

/**
 * @brief Método que para o veículo.
 * 
 */
void Motor::stop(){
	analogWrite(this->ENA, this->speed);
	analogWrite(this->ENB, this->speed);
	digitalWrite(this->IN1, LOW);
	digitalWrite(this->IN3, LOW);
	digitalWrite(this->IN2, LOW);
	digitalWrite(this->IN4, LOW);
}

/**
 * @brief Método que implementa curva à direita.
 * 
 */
void Motor::right(){
	analogWrite(this->ENA, this->speed);
	analogWrite(this->ENB, this->speed);
	digitalWrite(this->IN2, LOW);
	digitalWrite(this->IN1, HIGH);
	digitalWrite(this->IN3, LOW);
	digitalWrite(this->IN4, HIGH);
}

/**
 * @brief Método que implementa curva à esquerda.
 * 
 */
void Motor::left(){
	analogWrite(this->ENA, this->speed);
	analogWrite(this->ENB, this->speed);
	digitalWrite(this->IN1, LOW);
	digitalWrite(this->IN3, HIGH);
	digitalWrite(this->IN4, LOW);
	digitalWrite(this->IN2, HIGH);
}

/**
 * @brief Método que habilita os motores.
 * 
 */
void Motor::func(){
	analogWrite(this->ENA, this->speed);
	analogWrite(this->ENB, this->speed);
}

/**
 * @brief Método que define a velocidade do motor.
 * 
 * @param speed Velocidade a ser utilizada.
 */
void Motor::setSpeed(long speed){
	this->speed = speed;
	if (speed < 0) {
		this->speed = 0;
	}
	if (speed > 220) {
		this->speed = 220;
	}
}