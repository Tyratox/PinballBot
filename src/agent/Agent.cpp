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
#include <string>
#include <sstream>
#include <algorithm>

#include <Box2D/Box2D.h>

#include "Agent.h"
#include "State.h"
#include "../action/Action.h"

const int	Agent::STATES_TO_BACKPORT		= 50;

const float Agent::VALUE_ADJUST_FRACTION	= 0.35f;
const float Agent::EPSILON					= 0.15f;

Agent::Agent(std::vector<Action*> availableActions):
	availableActions(availableActions), generator(seed()){

	loadPolicyFromFile();
}

void Agent::think(State state, std::vector<float> collectedRewards){

	int		currentStateIndex	= 0;

	//first check whether this state already occured or not and retrieve the iterator
	std::vector<State>::iterator low = std::lower_bound(states.begin(), states.end(), state);

	if(low == states.end()){
		//If its a new state, add it
		states.push_back(state);
		currentStateIndex = states.size()-1;

		//and now sort it in order to be able to use binary search
		std::sort(states.begin(), states.end());

	}else{
		currentStateIndex = (low - states.begin());
	}

	//If there was a state before the current one (not the first); TODO Can be optimized, saves one if per loop
	if(lastActions.size() != 0){

		float lastValue, factor;

		/*
		 * Maybe (actually most of the time in a pinball game) the good/bad reward isn't simply caused by the last action taken
		 * A series of actions and event have lead to this specific situation, so we need to apply the reward received for the last action
		 * to the last state in general, in other words to every possible action
		 *
		 * Example:
		 * State 0:		Action: EnableFlipperLeft		Reward:	0.0f	Cause: Ball leaves the CF in the direction of a pin
		 * State 10:	Action: DisableFlipperRight		Reward: 0.0f	Cause: Nothing, ball is still travelling in the direction of the pin
		 * State 15:	Action: DisableFlipperRight		Reward: 1.0f	Cause: Nothing BUT the ball kicked in state 0 hits the pin and causes a reward of 1.0f
		 *
		 * Now we want to series of actions taken from the last few states to occur more often because we know it was "good"
		 * We now want to apply the reward received in state 15 to the actions taken in the last few steps
		 */

		for(int i=(lastActions.size() - 1);i>-1;i--){

			factor = ((float)i) / (lastActions.size() - 1);

			//If there isn't already a value set for the action taken in the last state, then set it to the default reward
			if(states[lastActions[i].first].values.find(lastActions[i].second) == states[lastActions[i].first].values.end()){
				states[lastActions[i].first].setValue(lastActions[i].second, Action::DEFAULT_REWARD);

				lastValue = Action::DEFAULT_REWARD;
			}else{
				lastValue = states[lastActions[i].first].getValue(lastActions[i].second);
			}

			//Apply all collected rewards, they can't be simply added up because then values greater than 1.0f would be possible
			for(int j=0;j<collectedRewards.size();j++){

				/* As currently we don't know more than that what we did in the last state and what the result is, we create a "connection" between the action and the reward
				 * If we receive a good reward (1.0f) the epsilonGreedy() function is more likely to select this action in exactly this state again
				 */
				lastValue = lastValue + (factor * VALUE_ADJUST_FRACTION) * (collectedRewards[j] - lastValue);
				states[lastActions[i].first].setValue(lastActions[i].second, lastValue);

				//printf("GOT NEW REWARD FOR ACTION: %s for STATE %d, old value: %f, new value: %f\n", lastAction->getUID(), lastStateIndex, lastValue, states[lastStateIndex].getValue(lastAction));

			}

			//TODO: can be optimized
			if(collectedRewards.size() == 0){
				/*
				 * If there was no reward we want to port the average value of the current state back
				 * in order to ensure it will occur more or less often. To do that, we simply converge all
				 * values of the previous [∆ - 2 - n] to the average of the last one [∆ - 1]
				 */

				lastValue = lastValue + (factor * VALUE_ADJUST_FRACTION) * (states[lastActions[lastActions.size()-1].first].getAverageValue() - lastValue);
				states[lastActions[i].first].setValue(lastActions[i].second, lastValue);

			}



		}

	}

	//then decide what action to take next
	Action* actionToTake = this->epsilonGreedy(states[currentStateIndex], EPSILON);

	//and the JUST DO IT
	actionToTake->run();

	lastActions.push_back(std::make_pair(currentStateIndex, actionToTake));

	if(lastActions.size() > STATES_TO_BACKPORT){
		for(int i=0;i<(lastActions.size() - STATES_TO_BACKPORT);i++){
			lastActions.pop_front();
		}
	}
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
	std::string					line, header;
	std::ifstream				policies;
	std::vector<std::string>	partials;

	states.clear();

	printf("Reading and parsing policies.csv....\n");

	policies.open("policies.csv");

	if(std::getline(policies, header)){
		while (std::getline(policies, line)){

			b2Vec2	ballPosition;
			b2Vec2	ballVelocity;

			split(line, ';', partials);

			ballPosition.x		= stof(partials[0]);
			ballPosition.y		= stof(partials[1]);

			ballVelocity.x		= stof(partials[2]);
			ballVelocity.y		= stof(partials[3]);

			State state(ballPosition, ballVelocity);

			for(int i=0; i<availableActions.size(); i++){ //TODO check if the order in the header is the same as in available actions
				state.values[availableActions[i]] = stof(partials[4 + i]);
			}

			// push new state to states
			states.push_back(state);

			partials.clear();
		}
	}

	printf("Read and parsed policies.csv, %lu states were imported.\n", states.size());

	return;
}

std::vector<std::string> Agent::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
