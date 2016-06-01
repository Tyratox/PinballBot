/*
 * Agent.cpp
 *
 * The reinforcement learning agent: the core of the project
 */

#include <vector>
#include <chrono>
#include <math.h>
#include <stdio.h>
#include <fstream>

#include "Agent.h"
#include "State.h"
#include "../action/Action.h"

const float Agent::VALUE_ADJUST_FRACTION	= 0.25;
const float Agent::EPSILON					= 0.15;

Agent::Agent(std::vector<Action*> availableActions):
	availableActions(availableActions), generator(seed()){
	lastStateIndex = -1;

	loadPolicyFromFile();
}

void Agent::think(State state, float reward){
	//first adjust the value for the taken action

	bool 	add = true;
	int		currentStateIndex = 0;

	for(int i=0;i<states.size();i++){

		//check if it is the "same" state
		if(states[i].ballPosition == state.ballPosition && states[i].ballVelocity == state.ballVelocity){
			add = false;
			currentStateIndex = i;
		}
	}

	if(add){
		states.push_back(state);
		currentStateIndex = states.size()-1;
	}

	if(lastStateIndex != -1){

		float lastValue;

		if(states[lastStateIndex].values.find(lastAction) == states[lastStateIndex].values.end()){
			states[lastStateIndex].setValue(lastAction, Action::DEFAULT_REWARD);
		}

		lastValue = states[lastStateIndex].getValue(lastAction);

		if(reward != Action::DEFAULT_REWARD){//ignore default values
			states[lastStateIndex].setValue(lastAction, lastValue + VALUE_ADJUST_FRACTION * (reward - lastValue));
			//printf("GOT NEW REWARD FOR ACTION: %s for STATE %d, old value: %f, new value: %f\n", lastAction->getUID(), lastStateIndex, lastValue, states[lastStateIndex].getValue(lastAction));
		}

		if(currentStateIndex != -1 && states[currentStateIndex].values.find(lastAction) != states[currentStateIndex].values.end()){

			float currentValue = states[currentStateIndex].getValue(lastAction);
			states[lastStateIndex].setValue(lastAction, states[lastStateIndex].getValue(lastAction) + VALUE_ADJUST_FRACTION * (currentValue - states[lastStateIndex].getValue(lastAction)));

		}

		if(states[lastStateIndex].getValue(lastAction) != Action::DEFAULT_REWARD){
			//printf("SET NEW VALUE FOR ACTION: %s for STATE %d, new value: %f\n", lastAction->getUID(), lastStateIndex, states[lastStateIndex].getValue(lastAction));
		}

	}

	//then decide what action to take next
	Action* actionToTake = this->epsilonGreedy(states[currentStateIndex], EPSILON);

	//and the JUST DO IT
	actionToTake->run();

	lastStateIndex	= currentStateIndex;
	lastAction		= actionToTake;
}

unsigned Agent::seed(){
	return (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
}

float Agent::randomFloatInRange(const float &min, const float &max){
	std::uniform_real_distribution<float>		distribution
	= std::uniform_real_distribution<float>(min, max);

	return distribution(generator);
}

int Agent::randomIntInRange(const int &min, const int &max){
	std::uniform_int_distribution<int>		distribution
		= std::uniform_int_distribution<int>(min, max);

	return distribution(generator);
}

/*Action Agent::softmax(State state, unsigned long temperature){

	std::vector<double> probabilities(states.size());
	float numerator, denominator;

	for(int i = 0;i<availableActions.size();i++){

	}

	for(int i=0;i<states.size();i++){

		probabilities[i] = 0;
	}
}*/

Action* Agent::epsilonGreedy(State state, const float &epsilon){
	if(epsilon < randomFloatInRange(0.0f, 1.0f)){
		//pick a greedy state
		return greedy(state);
	}else{
		//pick a random state
		return random(availableActions);
	}
}

Action* Agent::greedy(State state){
	float					maxValue = 0;
	float					tmpValue;
	std::vector<Action*>	maxActions;

	for(int i=0;i<availableActions.size();i++){
		tmpValue		= state.getValue(availableActions[i]);

		if(tmpValue > maxValue){
			maxValue = tmpValue;

			maxActions.clear();
			maxActions.push_back(availableActions[i]);
		}else if(tmpValue == maxValue){
			maxActions.push_back(availableActions[i]);
		}
	}

	if(maxActions.size() == 1){
		return maxActions[0];
	}else{
		return random(maxActions);
	}
}

Action* Agent::random(std::vector<Action*> availableActions){
	return availableActions[randomIntInRange(0, availableActions.size()-1)];
}

void Agent::savePoliciesToFile(){

	if(states.size() != 0){

		std::ofstream policies;
		policies.open("policies.csv");

		//Generate header
		policies << "POSITION_X;POSITION_Y;VELOCITY_X;VELOCITY_Y";
		for(int i=0;i<availableActions.size();i++){
			policies << ";ACTION_" << availableActions[i]->getUID();
		}
		policies << std::endl;

		//and the content in the same order

		for(int i=0;i<states.size();i++){
			policies << states[i].ballPosition.x << ";" << states[i].ballPosition.y << ";"
								<< states[i].ballVelocity.x << ";" << states[i].ballVelocity.y;

			for(int j=0;j<availableActions.size();j++){

				if(states[i].values.find(availableActions[j]) == states[i].values.end()){
					policies << ";" << Action::DEFAULT_REWARD;
					//printf("DEFAULT Value: %f\n", Action::DEFAULT_REWARD);
				}else{
					policies << ";" << (states[i].getValue(availableActions[j]));
					//printf("Value: %f\n", (states[i].getValue(availableActions[j])));
				}
			}

			policies << std::endl;
		}
	}
}

void Agent::loadPolicyFromFile(){

}
