/*
 * State.cpp
 *
 * Contains all the information of a state
 */

#include "State.h"
#include "Ball.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <random>


float State::roundFloat(const int &precision, const float &number){
	return round(number * (10 * precision)) / (10 * precision);
}

b2Vec2 State::reduceVectorPrecision(const int &precision, const b2Vec2 &vec){
	return b2Vec2(this->roundFloat(precision, vec.x), this->roundFloat(precision, vec.y));
}

void State::reduceStatePrecision(const int &precision){
	ball.setPosition(reduceVectorPrecision(precision, ball.getPosition()));
	ball.setVelocity(reduceVectorPrecision(precision, ball.getVelocity()));
}

State::State(Ball ball, bool isLeftFlipperActive, bool isRightFlipperActive) : isLeftFlipperActive(isLeftFlipperActive), isRightFlipperActive(isRightFlipperActive){
	this->ball = ball;
	reduceStatePrecision(2);

	std::cout << this->ball.getPosition().x << " " << this->ball.getPosition().y << std::endl;
}

float State::getReward(){

	return 0.0f;
}

float State::getValue(){

	return 0.0f;
}
