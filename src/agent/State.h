/*
 * State.h
 *
 * Contains all the information of a state
 */

#ifndef AGENT_STATE_H_
#define AGENT_STATE_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <Box2D/Box2D.h>

class State{

	private:

		b2Vec2 position;
		b2Vec2 velocity;

		bool isLeftFlipperActive;
		bool isRightFlipperActive;

		/**
		 * Reduces the precision of a floating point number in order to reduce the data load
		 * @param	precision	int			The amount of decimal places to round to
		 * @param	number		float		The floating point number to round
		 * @return				float		The rounded number
		 */
		float roundFloat(const int &precision, const float &number);

		/**
		 * Reduces the precision of a b2Vec
		 * @param	precision	int			The amount of decimal places to round floats to
		 * @param	vec			b2Vec2		The vector whose precision should be reduced
		 * @return				b2Vec2		The rounded number
		 */
		b2Vec2 reduceVectorPrecision(const int &precision, const b2Vec2 &vec);

		/**
		 * Reduces the precision of the state in order to reduce the data load
		 * @param	precision	int			The amount of decimal places to round floats to
		 * @return				void
		 */
		void reduceStatePrecision(const int &precision);


	public:

		/**
		 * Inits a state
		 * @param	ball					Ball		Data about the ball (position, velocity)
		 * @param	isLeftFlipperActive		bool		If the left		flipper currently is active
		 * @param	isRightFlipperActive	bool		If the right	flipper currently is active
		 */
		State(b2Vec2 position = b2Vec2(0, 0), b2Vec2 velocity = b2Vec2(0, 0), bool isLeftFlipperActive = false, bool isRightFlipperActive = false);

		/**
		 * Gets the expected reward based on a lookup table and the model of the environment
		 * @param	s	State	The State of which the reward is calculated
		 * @return		double	The expected reward
		 */

		float getReward();

		/**
		 * Calculates the expected value the given state will in the next turn and in the future => convergence
		 * @param	s	State	The State of which the value is calculated
		 * @return		double	The expected value
		 */

		float getValue();

};



#endif /* AGENT_STATE_H_ */
