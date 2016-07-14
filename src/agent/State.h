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

	private:
		class Coords{
			public:
				char x[8], y[8];

				Coords(const char* x = "0", const char* y = "0");

				bool operator==(const Coords& cmp){
					return strcmp(x, cmp.x) == 0 && strcmp(y, cmp.y) == 0;
				}
			};

	public:

		std::map<Action*, float> 		values;

		Coords							ballPosition;
		Coords							ballVelocity;

		/**
		 * Inits a state
		 * @param	ballPosition		b2Vec2					The ball position
		 * @param	ballVelocity		b2Vec2					The ball velocity
		 */
		State(const b2Vec2 ballPosition = b2Vec2(0, 0), const b2Vec2 ballVelocity = b2Vec2(0, 0));

		/**
		 * Gets the expected reward if a specific action is taken
		 * @param	action	*Action	The action of which the expected reward is returned
		 * @return			float	The expected reward
		 */

		float getValue(Action *action);

		/**
		 * Gets the average value
		 * @return			float	The expected reward
		 */

		float getAverageValue();

		/**
		 * Sets the expected reward if a specific action is taken
		 * @param	action	*Action	The action of which the expected reward is set
		 * @param	value	float	The expected reward
		 * @return			void
		 */

		void setValue(Action *action, float value);

		/**
		 * Rounds a float to a char
		 * @param	f				float32	The floating point number to round
		 * @return					char*
		 */
		char* round(float32 f);


		bool operator==(const State& cmp){
			return ballPosition == cmp.ballPosition && ballVelocity == cmp.ballVelocity;
		}
};



#endif /* AGENT_STATE_H_ */
