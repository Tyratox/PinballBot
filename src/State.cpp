/*
 * State.cpp
 * Contains all the information of a state
 *
 */

#ifndef PINBALL_BOT_STATE
#define PINBALL_BOT_STATE

#include "Ball.cpp"

class State{

	private:

		std::vector<Ball> balls;

	public:

		/**
		 * Gets the expected reward based on a lookup table and the model of the environment
		 * @param	s	State	The State of which the reward is calculated
		 * @return		double	The expected reward
		 */

		double reward(State s){



			return 0.0;
		}

		/**
		 * Calculates the expected value the given state will in the next turn and in the future.
		 * Policies, the reward function and the model of the environment are used to do this calculation.
		 * @param	s	State	The State of which the value is calculated
		 * @return		double	The expected value
		 */

		double value(State s){


			return 0.0;
		}

};

#endif /* PINBALL_BOT_STATE */
