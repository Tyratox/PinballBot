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

void Agent::savePoliciesToFile(){

	if(states.size() != 0){

		Json::Value policies;

		for(int i=0;i<states.size();i++){
			Json::Value state;

			state["position"]["x"] = states[i].ballPosition.x;
			state["position"]["y"] = states[i].ballPosition.y;

			state["velocity"]["x"] = states[i].ballVelocity.x;
			state["velocity"]["y"] = states[i].ballVelocity.y;

			for(auto const &iterator : states[i].values) {
				state["values"][iterator.first->getUID()] = iterator.second;
			}
			policies["states"][i] = state;
		}

		std::ofstream file_id;
		file_id.open("policies.json");

		Json::StyledWriter styledWriter;
		file_id << styledWriter.write(policies);

		file_id.close();
	}
}

void Agent::loadPolicyFromFile(){
	Json::Value root; //will be populated after parsing
	Json::Reader reader;
	std::ifstream policies("policies.json", std::ifstream::binary);

	if (reader.parse(policies, root, false)){

		states = std::vector<State>(root["states"].size());

		for(int i=0;i<root["states"].size();i++){
			State state;

			state.ballPosition = b2Vec2(std::stof(root["states"][i]["position"]["x"].asString()), std::stof(root["states"][i]["position"]["y"].asString()));
			state.ballVelocity = b2Vec2(std::stof(root["states"][i]["velocity"]["x"].asString()), std::stof(root["states"][i]["velocity"]["y"].asString()));

			for(Json::ValueIterator itr = root["states"][i]["values"].begin() ; itr != root["states"][i]["values"].end() ; itr++){
				for(int j=0;j<availableActions.size();j++){
					if(strcmp(itr.key().asCString(), availableActions[j]->getUID()) == 0){
						state.values[availableActions[i]] = std::stof(root["states"][i]["values"][itr.key().asString()].asString());
					}
				}
			}

			states[i] = state;
		}

	}else{
		std::cout  << reader.getFormattedErrorMessages() << std::endl;
	}


}
