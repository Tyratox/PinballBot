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

State::State(b2Vec2 ballPosition, b2Vec2 ballVelocity, std::vector<Action*> availableActions){
	this->ballPosition = Coords(round(ballPosition.x), round(ballPosition.y));
	this->ballVelocity = Coords(round(ballVelocity.x), round(ballVelocity.y));

	for(int i=0;i<availableActions.size();i++){
		setValue(availableActions[i], Action::DEFAULT_REWARD);
	}
}

State::Coords::Coords(const char* x, const char* y) : x{'0'}, y{'0'}{
	strcpy(this->x, x);
	strcpy(this->y, y);
}

float State::getValue(Action *action){
	return values[action];
}

float State::getAverageValue(){
	float average = 0;

	for(auto const &iter : values){
		average += iter.second;
	}

	return (average / values.size());
}

void State::setValue(Action *action, float value){
	values[action] = value;
}

char* State::round(float32 f){
	if(f > 10){f = 0;}

	char buf[8];
	sprintf(buf, "%.2f", f);
	return strdup(buf);
}

void State::debug(){
	for(auto const &iter : values){
		printf("%s : %f\n", iter.first->getUID(), iter.second);
	}
}

bool operator==(const State::Coords& lhs, const State::Coords& rhs){
	return strcmp(lhs.x, rhs.x) == 0 && strcmp(lhs.y, rhs.y) == 0;
}
bool operator!=(const State::Coords& lhs, const State::Coords& rhs){
	return strcmp(lhs.x, rhs.x) != 0 || strcmp(lhs.y, rhs.y) != 0;
}


bool operator==(const State& lhs, const State& rhs){
	return lhs.ballPosition == rhs.ballPosition && lhs.ballVelocity == rhs.ballVelocity;
}

bool operator!=(const State& lhs, const State& rhs){
	return !(lhs == rhs);
}

bool operator>(const State& lhs, const State& rhs){
	if(lhs.ballPosition.x != rhs.ballPosition.x){
		return lhs.ballPosition.x > rhs.ballPosition.x;

	}else if(lhs.ballPosition.y != rhs.ballPosition.y){
		return lhs.ballPosition.y > rhs.ballPosition.y;

	}else if(lhs.ballVelocity.x != rhs.ballVelocity.x){
		return lhs.ballVelocity.x > rhs.ballVelocity.x;

	}else if(lhs.ballVelocity.y != rhs.ballVelocity.y){
		return lhs.ballVelocity.y > rhs.ballVelocity.y;

	}else{
		return false;
	}
}

bool operator<(const State& lhs, const State& rhs){
	if(lhs.ballPosition.x != rhs.ballPosition.x){
		return lhs.ballPosition.x < rhs.ballPosition.x;

	}else if(lhs.ballPosition.y != rhs.ballPosition.y){
		return lhs.ballPosition.y < rhs.ballPosition.y;

	}else if(lhs.ballVelocity.x != rhs.ballVelocity.x){
		return lhs.ballVelocity.x < rhs.ballVelocity.x;

	}else if(lhs.ballVelocity.y != rhs.ballVelocity.y){
		return lhs.ballVelocity.y < rhs.ballVelocity.y;

	}else{
		return false;
	}
}

bool operator<=(const State& lhs, const State& rhs){
	return !(lhs > rhs);
}

bool operator>=(const State& lhs, const State& rhs){
	return !(lhs < rhs);
}
