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

State::State(b2Vec2 ballPosition, b2Vec2 ballVelocity, const std::vector<Action*> &availableActions) : ballPosition(ballPosition), ballVelocity(ballVelocity){

}

float State::getValue(Action *action){
	return values[action];
}

void State::setValue(Action *action, float value){
	values[action] = value;
}
