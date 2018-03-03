#include "Follow.h"

Follow::Follow(int sensor){
	this->sensor = sensor;
}

void Follow::setup(){
	pinMode(this->sensor, INPUT);
}

int Follow::read(){
	return analogRead(this->sensor);
}

bool Follow::alert(int treshold, int sensor){
	// DEBUG
	// Serial.print("debug ");Serial.print(sensor);Serial.print(": ");
	// Serial.println(read());

	if(read() > treshold){
		return true;
	}
	return false;
}

bool Follow::checkLL(int notBlack) {
	// DEBUG
	// Serial.print("LL: ");
	// Serial.println(read());

	if (read() < notBlack) {
		return true;
	} 
	return false;
}