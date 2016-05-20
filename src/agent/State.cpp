/*
 * State.cpp
 *
 * Contains all the information of a state
 */

#include "State.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <Box2D/Box2D.h>


float State::roundFloat(const int &precision, const float &number){
	return round(number * (10 * precision)) / (10 * precision);
}

b2Vec2 State::reduceVectorPrecision(const int &precision, const b2Vec2 &vec){
	return b2Vec2(this->roundFloat(precision, vec.x), this->roundFloat(precision, vec.y));
}

void State::reduceStatePrecision(const int &precision){
	position = reduceVectorPrecision(precision, position);
	velocity = reduceVectorPrecision(precision, velocity);
}

State::State(b2Vec2 position, b2Vec2 velocity, bool isLeftFlipperActive, bool isRightFlipperActive) : position(position), velocity(velocity), isLeftFlipperActive(isLeftFlipperActive), isRightFlipperActive(isRightFlipperActive){
	reduceStatePrecision(2);
}

float State::getReward(){

	return 0.0f;
}

float State::getValue(){

	return 0.0f;
}
