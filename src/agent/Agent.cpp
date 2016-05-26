/*
 * Agent.cpp
 *
 * The reinforcement learning agent: the core of the project
 */

#include <vector>
#include <chrono>
#include <math.h>
#include <stdio.h>

#include "Agent.h"
#include "State.h"
#include "../action/Action.h"

const float Agent::VALUE_ADJUST_FRACTION	= 0.25;
const float Agent::EPSILON					= 0.15;

Agent::Agent(std::vector<Action*> availableActions):
	availableActions(availableActions), generator(seed()){
	lastStateIndex = -1;
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

	if(reward != Action::DEFAULT_REWARD && lastStateIndex != -1){ //ignore default values
		int lastValue = states[currentStateIndex].getValue(lastAction);
		states[lastStateIndex].setValue(lastAction, lastValue + VALUE_ADJUST_FRACTION * (reward - lastValue));
	}

	//then decide what action to take next
	Action* actionToTake = this->epsilonGreedy(state, EPSILON);

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

void Agent::savePolicyToFile(){
	Json::Value policy;
	for(int i=0;i<states.size();i++){
		Json::Value state;

		state["position"]["x"] = std::to_string(states[i].ballPosition.x).substr(0,5);
		state["position"]["y"] = std::to_string(states[i].ballPosition.y).substr(0,5);

		state["velocity"]["x"] = std::to_string(states[i].ballVelocity.x).substr(0,5);
		state["velocity"]["y"] = std::to_string(states[i].ballVelocity.y).substr(0,5);

		for(auto const &iterator : states[i].values) {
			state["values"][iterator.first->getUID()] = std::to_string(iterator.second).substr(0,5);
		}

		policy["states"][i] = state;
	}

	std::cout << policy << std::endl;
}
