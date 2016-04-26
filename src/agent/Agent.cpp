/*
 * Agent.cpp
 *
 * The reinforcement learning agent: the core of the project
 */

#ifndef PINBALL_BOT_AGENT
#define PINBALL_BOT_AGENT

#include <vector>

#include "Policy.cpp"
#include "State.cpp"
#include "../action/Action.cpp"

class Agent{

	private:

		std::vector<Action> availableActions;

		Policy policy;

		/**
		 * Generates a seed.
		 * @return	void
		 */
		static unsigned int seed(){
			return (unsigned int) rand();//FIXME not the best solution but works for now.
		}

		/**
		 * Generates a pseudo-random float
		 * @param	min		float		The minimum possible number to generate [included]
		 * @param	max		float		The maximum possible number to generate [included]
		 * @return			float		The pseudo random number
		 */
		static float randomFloatInRange(const float &min, const float &max){
			std::default_random_engine					generator(seed());

			std::uniform_real_distribution<float>		distribution
			= std::uniform_real_distribution<float>(min, max);

			return distribution(generator);
		}

		/**
		 * Generates a pseudo-random int
		 * @param	min		int			The minimum possible number to generate [included]
		 * @param	max		int			The maximum possible number to generate [included]
		 * @return			int			The pseudo random number
		 */
		static int randomIntInRange(const int &min, const int &max){
			std::default_random_engine					generator(seed());

			std::uniform_int_distribution<int>		distribution
				= std::uniform_int_distribution<int>(min, max);

			return distribution(generator);
		}

		/**
		 * Returns one state inside of a vector based on a epsilon greedy algorithm
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @param	epsilon		float					Range: [0-1]: The percentage of time which this function should pick a random state
		 * @return				State					The picked state
		 */
		static State epsilonGreedy(std::vector<State> states, const float &epsilon){
			if(epsilon < randomFloatInRange(0.0f, 1.0f)){
				//pick a greedy state
				return greedy(states);
			}else{
				//pick a random state
				return random(states);
			}
		}

		/**
		 * Picks the state with the highest value
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @return				State					The picked state
		 */
		static State greedy(std::vector<State> states){
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

		/**
		 * Picks a random state
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @return				State					The picked state
		 */
		static State random(const std::vector<State> &states){
			return states[randomIntInRange(0, states.size()-1)];
		}

	public:

		/**
		 * Init the Agent class
		 * @param	availableActions	std::vector<Action>		The actions available to the agent
		 * @param	policy				Policy					A previously saved policy (optional)
		 */
		Agent(std::vector<Action> availableActions, Policy policy = Policy()) : availableActions(availableActions), policy(policy){

		}

		/**
		 * Here the agent decides what to do based on a State s
		 * @param	s		State		The state on which the decision is based
		 */
		void think(State s){

		}

};

#endif /* PINBALL_BOT_AGENT */
