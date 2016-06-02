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
