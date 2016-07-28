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

		int								ballPosition_x;
		int								ballPosition_y;

		int								ballVelocity_x;
		int								ballVelocity_y;

		/**
		 * Inits a state
		 * @param	ballPosition		b2Vec2					The ball position
		 * @param	ballVelocity		b2Vec2					The ball velocity
		 */
		State(const b2Vec2 ballPosition = b2Vec2(0, 0), const b2Vec2 ballVelocity = b2Vec2(0, 0), std::vector<Action*> availableActions = std::vector<Action*>(0));

		/**
		 * Inits a state
		 * @param	ballPosition_x		int						The balls x position
		 * @param	ballPosition_y		int						The balls y position
		 * @param	ballVelocity_x		int						The balls x velocity
		 * @param	ballVelocity_y		int						The balls y velocity
		 */
		State(int ballPosition_x = 0, int ballPosition_y = 0, int ballVelocity_x = 0, int ballVelocity_y = 0, std::vector<Action*> availableActions = std::vector<Action*>(0));

		/**
		 * Gets the expected reward if a specific action is taken
		 * @param	action	*Action	The action of which the expected reward is returned
		 * @return			float	The expected reward
		 */

		float getValue(Action *action);

		/**
		 * Gets the "general" value
		 * @return			float	The expected reward
		 */

		float getGeneralValue();

		/**
		 * Sets the expected reward if a specific action is taken
		 * @param	action	*Action	The action of which the expected reward is set
		 * @param	value	float	The expected reward
		 * @return			void
		 */

		void setValue(Action *action, float value);

		/**
		 * Rounds a float to a char (with "position precision")
		 * @param	f				float32	The floating point number to round
		 * @return					int
		 */
		int roundPos(float32 f);

		/**
		 * Rounds a float to a char (with "velocity precision")
		 * @param	f				float32	The floating point number to round
		 * @return					int
		 */
		int roundVel(float32 f);

		/**
		 * Prints some debugging values
		 * @return		void
		 */
		void debug();
};

bool operator ==	(const State & lhs, const State & rhs);
bool operator !=	(const State & lhs, const State & rhs);
bool operator <		(const State & lhs, const State & rhs);
bool operator <=	(const State & lhs, const State & rhs);
bool operator >		(const State & lhs, const State & rhs);
bool operator >=	(const State & lhs, const State & rhs);



#endif /* AGENT_STATE_H_ */
