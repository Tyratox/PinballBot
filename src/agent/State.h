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
#include <map>

#include <Box2D/Box2D.h>

#include "../action/Action.h"

class State{

	public:

		static const int	FLOAT_PRECISION;

	private:

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

		std::map<Action*, float> 		values;

		b2Vec2							ballPosition;
		b2Vec2							ballVelocity;

		/**
		 * Inits a state
		 * @param	ballPosition		b2Vec2					The ball position
		 * @param	ballVelocity		b2Vec2					The ball velocity
		 * @param	availableActions	std::vector<Action*>	The actions available, needed for the setting the default values of the 'values' map
		 */
		State(const b2Vec2 ballPosition = b2Vec2(0, 0), const b2Vec2 ballVelocity = b2Vec2(0, 0), const std::vector<Action*> &availableActions = std::vector<Action*>());

		/**
		 * Gets the expected reward if a specific action is taken
		 * @param	action	*Action	The action of which the expected reward is returned
		 * @return			float	The expected reward
		 */

		float getValue(Action *action);

		/**
		 * Sets the expected reward if a specific action is taken
		 * @param	action	*Action	The action of which the expected reward is set
		 * @param	value	float	The expected reward
		 * @return			void
		 */

		void setValue(Action *action, float value);

};



#endif /* AGENT_STATE_H_ */
