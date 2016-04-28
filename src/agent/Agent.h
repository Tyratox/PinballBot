/*
 * Agent.h
 *
 * The reinforcement learning agent: the core of the project
 */

#ifndef AGENT_AGENT_H_
#define AGENT_AGENT_H_

#include <vector>

#include "Policy.h"
#include "State.h"
#include "../action/Action.h"

class Agent{

	private:

		std::vector<Action> availableActions;

		Policy policy;

		/**
		 * Generates a seed.
		 * @return	void
		 */
		static unsigned int seed();

		/**
		 * Generates a pseudo-random float
		 * @param	min		float		The minimum possible number to generate [included]
		 * @param	max		float		The maximum possible number to generate [included]
		 * @return			float		The pseudo random number
		 */
		static float randomFloatInRange(const float &min, const float &max);

		/**
		 * Generates a pseudo-random int
		 * @param	min		int			The minimum possible number to generate [included]
		 * @param	max		int			The maximum possible number to generate [included]
		 * @return			int			The pseudo random number
		 */
		static int randomIntInRange(const int &min, const int &max);

		/**
		 * Returns one state inside of a vector based on a epsilon greedy algorithm
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @param	epsilon		float					Range: [0-1]: The percentage of time which this function should pick a random state
		 * @return				State					The picked state
		 */
		static State epsilonGreedy(std::vector<State> states, const float &epsilon);

		/**
		 * Picks the state with the highest value
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @return				State					The picked state
		 */
		static State greedy(std::vector<State> states);

		/**
		 * Picks a random state
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @return				State					The picked state
		 */
		static State random(const std::vector<State> &states);

	public:

		/**
		 * Init the Agent class
		 * @param	availableActions	std::vector<Action>		The actions available to the agent
		 * @param	policy				Policy					A previously saved policy (optional)
		 */
		Agent(std::vector<Action> availableActions, Policy policy = Policy());

		/**
		 * Here the agent decides what to do based on a State s
		 * @param	s		State		The state on which the decision is based
		 */
		void think(State s);

};



#endif /* AGENT_AGENT_H_ */
