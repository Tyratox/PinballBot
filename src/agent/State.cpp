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

State::State(b2Vec2 ballPosition, b2Vec2 ballVelocity){
	this->ballPosition = Coords(round(ballPosition.x), round(ballPosition.y));
	this->ballVelocity = Coords(round(ballVelocity.x), round(ballVelocity.y));
}

State::Coords::Coords(const char* x, const char* y) : x{'0'}, y{'0'}{
	strcpy(this->x, x);
	strcpy(this->y, y);
}

char* State::round(float32 f){
	if(f > 10){f = 0;}

	char buf[8];
	sprintf(buf, "%.2f", f);
	return strdup(buf);
}

float State::getValue(Action *action){
	return values[action];
}

void State::setValue(Action *action, float value){
	values[action] = value;
}
