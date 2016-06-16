/*
 * Agent.h
 *
 * The reinforcement learning agent: the core of the project
 */

#ifndef AGENT_AGENT_H_
#define AGENT_AGENT_H_

#include <vector>
#include <iostream>

#include <Box2D/Box2D.h>

#include "State.h"
#include "../action/Action.h"

class Agent{

	public:
		static const float					FLOAT_COMPARE_EPSILON;
		static const float					VALUE_ADJUST_FRACTION;
		static const float					EPSILON;

	private:

		std::vector<Action*>				availableActions;
		std::default_random_engine			generator;

		/**
		 * Generates a seed.
		 * @return	void
		 */
		static unsigned seed();

		/**
		 * Generates a pseudo-random float
		 * @param	min		float		The minimum possible number to generate [included]
		 * @param	max		float		The maximum possible number to generate [included]
		 * @return			float		The pseudo random number
		 */
		float randomFloatInRange(const float &min, const float &max);

		/**
		 * Generates a pseudo-random int
		 * @param	min		int			The minimum possible number to generate [included]
		 * @param	max		int			The maximum possible number to generate [included]
		 * @return			int			The pseudo random number
		 */
		int randomIntInRange(const int &min, const int &max);

		/**
		 * Returns one state inside of a vector based on a softmax algorithm
		 * @param	state			State					A state containing all the possible actions
		 * @param	temperature		unsigned long			High "temperature" => all actions equally probable, low "temperature" => big difference, 0 = greedy
		 * @return					Action					The picked action
		 */
		//Action softmax(State state, unsigned long temperature);

		/**
		 * Returns one state inside of a vector based on a epsilon greedy algorithm
		 * @param	state		State					A state containing all the possible actions
		 * @param	epsilon		float					Range: [0-1]: The percentage of time which this function should pick a random state
		 * @return				Action					The picked action
		 */
		Action* epsilonGreedy(State state, const float &epsilon);

		/**
		 * Picks the state with the highest value
		 * @param	state		State					A state containing all the possible actions
		 * @return				Action					The picked action
		 */
		Action* greedy(State state);

		/**
		 * Picks a random state
		 * @param	states		std::vector<Action*>	A vector containing all the possible actions
		 * @return				Action					The picked action
		 */
		Action* random(std::vector<Action*> availableActions);

	public:

		std::vector<State>					states;

		int									lastStateIndex;
		Action*								lastAction;

		/**
		 * Init the Agent class
		 * @param	availableActions	std::vector<Action*>	The actions available to the agent
		 */
		Agent(std::vector<Action*> availableActions);

		/**
		 * Based on a given state the agent needs to decide what to do
		 * @param	state		State		The given state
		 * @param	reward		float		The returned reward
		 * @return				void
		 */
		void think(State s, std::vector<float> collectedRewards);

		/**
		 * Saves the policy to a file
		 */

		void savePoliciesToFile();

		/**
		 * Loads a policy file
		 */

		void loadPolicyFromFile();

		/**
		* Breaks a String into partials beteen the delimiter
		* @param	s		String			String to break down
		* @param	delim	char			Deimiter between partials
		* @param	elems	vector<string>	vector where strings are saved into
		* @return			vector<string>
		*/
		std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);

		/**
		 * Compares two floats based on an epsilon
		 * @param	a		float	The first float to compare
		 * @param	b		float	The second float to compare
		 * @return			float
		 */
		bool areEqualEpsilon(float a, float b);

};



#endif /* AGENT_AGENT_H_ */
