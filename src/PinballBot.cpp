//============================================================================
// Name        : PinballBot.cpp
// Author      : Nico Hauser, David Schmid
// Version     : 0.1
// Description : A reinforcement learning agent that learns to play pinball
//============================================================================

#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

/**
 * Contains all the information of a state
 */

class State{

};

/**
 * Gets the expected reward based on a lookup table and the model of the environment
 * @param	s			State		The State of which the reward is calculated
 * @return				double		The expected reward
 */

double reward(State s){



	return 0.0;
}

/**
 * Calculates the expected value the given state will in the next turn and in the future.
 * Policies, the reward function and the model of the environment are used to do this calculation.
 * @param	s			State		The State of which the value is calculated
 * @return				double		The expected value
 */

double value(State s){


	return 0.0;
}

int main() {

	boost::numeric::ublas::vector<double> v(3);

	v[0] = 10;

	std::cout << v[0] << std::endl;



	return 0;
}
