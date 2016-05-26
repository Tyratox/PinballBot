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

const int State::FLOAT_PRECISION	= 3;

float State::roundFloat(const int &precision, const float &number){
	return round(number * (10 * precision)) / (10 * precision);
}

b2Vec2 State::reduceVectorPrecision(const int &precision, const b2Vec2 &vec){
	return b2Vec2(this->roundFloat(precision, vec.x), this->roundFloat(precision, vec.y));
}

void State::reduceStatePrecision(const int &precision){
	ballPosition = reduceVectorPrecision(precision, ballPosition);
	ballVelocity = reduceVectorPrecision(precision, ballVelocity);
}

State::State(b2Vec2 ballPosition, b2Vec2 ballVelocity, const std::vector<Action*> &availableActions) : ballPosition(ballPosition), ballVelocity(ballVelocity){
	reduceStatePrecision(FLOAT_PRECISION);
}

float State::getValue(Action *action){
	return values[action];
}

void State::setValue(Action *action, float value){
	values[action] = value;
}
