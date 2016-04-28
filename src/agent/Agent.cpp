/*
 * Agent.cpp
 *
 * The reinforcement learning agent: the core of the project
 */

#include <vector>

#include "Agent.h"
#include "Policy.h"
#include "State.h"
#include "../action/Action.h"


unsigned int Agent::seed(){
	return (unsigned int) rand();//FIXME not the best solution but works for now.
}

float Agent::randomFloatInRange(const float &min, const float &max){
	std::default_random_engine					generator(seed());

	std::uniform_real_distribution<float>		distribution
	= std::uniform_real_distribution<float>(min, max);

	return distribution(generator);
}

int Agent::randomIntInRange(const int &min, const int &max){
	std::default_random_engine					generator(seed());

	std::uniform_int_distribution<int>		distribution
		= std::uniform_int_distribution<int>(min, max);

	return distribution(generator);
}

State Agent::epsilonGreedy(std::vector<State> states, const float &epsilon){
	if(epsilon < randomFloatInRange(0.0f, 1.0f)){
		//pick a greedy state
		return greedy(states);
	}else{
		//pick a random state
		return random(states);
	}
}

State Agent::greedy(std::vector<State> states){
	float				maxValue = 0;
	float				tmpValue;
	std::vector<State>	maxStates;

	for(int i=1;i<states.size();i++){
		tmpValue		= states[i].getValue();

		if(tmpValue > maxValue){
			maxValue = tmpValue;

			maxStates.clear();
			maxStates.push_back(states[i]);
		}else if(tmpValue == maxValue){
			maxStates.push_back(states[i]);
		}
	}

	if(maxStates.size() == 1){
		return maxStates[0];
	}else{
		return random(maxStates);
	}
}

State Agent::random(const std::vector<State> &states){
	return states[randomIntInRange(0, states.size()-1)];
}

Agent::Agent(std::vector<Action> availableActions, Policy policy) : availableActions(availableActions), policy(policy){

}

void Agent::think(State s){

}
