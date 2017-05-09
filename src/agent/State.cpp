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

	ballPosition_x	= roundPos(ballPosition.x);
	ballPosition_y	= roundPos(ballPosition.y);

	ballVelocity_x	= roundVel(ballVelocity.x);
	ballVelocity_x	= roundVel(ballVelocity.y);

	for(int i=0;i<availableActions.size();i++){
		setValue(availableActions[i], Action::DEFAULT_REWARD);
	}
}

State::State(int ballPosition_x, int ballPosition_y,
				int ballVelocity_x, int ballVelocity_y,
				std::vector<Action*> availableActions) :

				ballPosition_x(ballPosition_x), ballPosition_y(ballPosition_y),
				ballVelocity_x(ballVelocity_x), ballVelocity_y(ballPosition_y)
		{

	for(int i=0;i<availableActions.size();i++){
		setValue(availableActions[i], Action::DEFAULT_REWARD);
	}
}

float State::getValue(Action *action){
	return values[action];
}

float State::getGeneralValue(){
	float max = 0;

	for(auto const &iter : values){
		if(iter.second > max){
			max = iter.second;
		}
	}

	return max;
}

void State::setValue(Action *action, float value){
	values[action] = value;
}

int State::roundPos(float32 f){
	if(f > 10){f = 0;}
	return (int) std::round(f * 100);
}

int State::roundVel(float32 f){
	if(f > 10){f = 0;}
	return (int) std::round(f * 10);
}

void State::debug(){
	printf("POS_x: %d, POS_y_ %d, VEL_x: %d, VEL_y: %d | ", ballPosition_x, ballPosition_y, ballVelocity_x, ballVelocity_y);
	for(auto const &iter : values){
		printf("%s : %f;", iter.first->getUID(), iter.second);
	}
	printf("\n");
}

bool operator==(const State& lhs, const State& rhs){
	return lhs.ballPosition_x == rhs.ballPosition_x
			&& lhs.ballPosition_y == rhs.ballPosition_y
			&& lhs.ballVelocity_x == rhs.ballVelocity_y;
}

bool operator!=(const State& lhs, const State& rhs){
	return !(lhs == rhs);
}

bool operator>(const State& lhs, const State& rhs){
	if(lhs.ballPosition_x != rhs.ballPosition_x){
		return lhs.ballPosition_x > rhs.ballPosition_x;

	}else if(lhs.ballPosition_y != rhs.ballPosition_y){
		return lhs.ballPosition_y > rhs.ballPosition_y;

	}else if(lhs.ballVelocity_x != rhs.ballVelocity_x){
		return lhs.ballVelocity_x > rhs.ballVelocity_x;

	}else if(lhs.ballVelocity_y != rhs.ballVelocity_y){
		return lhs.ballVelocity_y > rhs.ballVelocity_y;

	}else{
		return false;
	}
}

bool operator<(const State& lhs, const State& rhs){
	if(lhs.ballPosition_x != rhs.ballPosition_x){
		return lhs.ballPosition_x < rhs.ballPosition_x;

	}else if(lhs.ballPosition_y != rhs.ballPosition_y){
		return lhs.ballPosition_y < rhs.ballPosition_y;

	}else if(lhs.ballVelocity_x != rhs.ballVelocity_x){
		return lhs.ballVelocity_x < rhs.ballVelocity_x;

	}else if(lhs.ballVelocity_y != rhs.ballVelocity_y){
		return lhs.ballVelocity_y < rhs.ballVelocity_y;

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
