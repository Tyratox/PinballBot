/*
 * Agent.h
 *
 * The reinforcement learning agent: the core of the project
 */

#ifndef AGENT_AGENT_H_
#define AGENT_AGENT_H_

#include <vector>
#include <string>
#include <deque>
#include <iostream>

#include <Box2D/Box2D.h>

#include "State.h"
#include "../action/Action.h"

class Agent{

	public:
		static const int					DEFAULT_STATES_TO_BACKPORT;

		static const float					DEFAULT_VALUE_ADJUST_FRACTION;
		static const float					DEFAULT_EPSILON;

		static const std::string			POLICIES_HEADER_POSITION_X;
		static const std::string			POLICIES_HEADER_POSITION_Y;
		static const std::string			POLICIES_HEADER_VELOCITY_X;
		static const std::string			POLICIES_HEADER_VELOCITY_Y;

		static const int					POLICIES_HEADER_ACTIONS_OFFSET;
		static const std::string			POLICIES_HEADER_ACTION_PREFIX;

	private:

		const int							STATES_TO_BACKPORT;

		const float							VALUE_ADJUST_FRACTION;
		const float							EPSILON;

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

		std::deque<std::pair<int, Action*>>	lastActions;

		/**
		 * Inits the Agent class
		 * @param	statesToBackport	int						The amount of states a reward will be backported
		 * @param	valueAdjustFraction	float					The fraction of the difference that will be added to the value
		 * @param	epsilon				float					The chance the agent will choose an action at random; range: [0.0 - 1.0]
		 * @param	availableActions	std::vector<Action*>	The actions available to the agent
		 */
		Agent(int statesToBackport = DEFAULT_STATES_TO_BACKPORT, float valueAdjustFraction = DEFAULT_VALUE_ADJUST_FRACTION, float epsilon = DEFAULT_EPSILON, std::vector<Action*> availableActions = std::vector<Action*>(0));

		/**
		 * Based on a given state the agent needs to decide what to do
		 * @param	state		State		The given state
		 * @param	reward		float		The returned reward
		 * @return				void
		 */
		void think(State s, std::vector<float> collectedRewards);

		/**
		 * Clears "useless" (all values = default)
		 * @return void
		 */
		void clearStates();

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

};



#endif /* AGENT_AGENT_H_ */
