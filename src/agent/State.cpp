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
	this->ballPosition_f 	= ballPosition;
	this->ballVelocity_f 	= ballVelocity;

	this->ballPosition		= Coords(roundPos(ballPosition.x), roundPos(ballPosition.y));
	this->ballVelocity		= Coords(roundVel(ballVelocity.x), roundVel(ballVelocity.y));

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
	float	average = 0;
	int		count	= 0;

	for(auto const &iter : values){
		if(iter.second != Action::DEFAULT_REWARD){
			average += iter.second;
			count++;
		}
	}

	return count == 0 ? Action::DEFAULT_REWARD : (average / count);
}

void State::setValue(Action *action, float value){
	values[action] = value;
}

char* State::roundPos(float32 f){
	if(f > 10){f = 0;}

	char buf[8];
	sprintf(buf, "%.2f", f);
	return strdup(buf);
}

char* State::roundVel(float32 f){
	if(f > 10){f = 0;}

	char buf[8];
	sprintf(buf, "%.1f", f);
	return strdup(buf);
}

void State::debug(){
	printf("POS_X: %s, POS_Y_ %s, VEL_X: %s, VEL_Y: %s | ", ballPosition.x, ballPosition.y, ballVelocity.x, ballVelocity.y);
	for(auto const &iter : values){
		printf("%s : %f;", iter.first->getUID(), iter.second);
	}
	printf("\n");
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
		return lhs.ballPosition_f.x > rhs.ballPosition_f.x;

	}else if(lhs.ballPosition.y != rhs.ballPosition.y){
		return lhs.ballPosition_f.y > rhs.ballPosition_f.y;

	}else if(lhs.ballVelocity.x != rhs.ballVelocity.x){
		return lhs.ballVelocity_f.x > rhs.ballVelocity_f.x;

	}else if(lhs.ballVelocity.y != rhs.ballVelocity.y){
		return lhs.ballVelocity_f.y > rhs.ballVelocity_f.y;

	}else{
		return false;
	}
}

bool operator<(const State& lhs, const State& rhs){
	if(lhs.ballPosition.x != rhs.ballPosition.x){
		return lhs.ballPosition_f.x < rhs.ballPosition_f.x;

	}else if(lhs.ballPosition.y != rhs.ballPosition.y){
		return lhs.ballPosition_f.y < rhs.ballPosition_f.y;

	}else if(lhs.ballVelocity.x != rhs.ballVelocity.x){
		return lhs.ballVelocity_f.x < rhs.ballVelocity_f.x;

	}else if(lhs.ballVelocity.y != rhs.ballVelocity.y){
		return lhs.ballVelocity_f.y < rhs.ballVelocity_f.y;

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
