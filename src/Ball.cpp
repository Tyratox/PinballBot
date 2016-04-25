/*
 * Ball.cpp
 * Stores information about a ball
 *
 */

#ifndef PINBALL_BOT_BALL
#define PINBALL_BOT_BALL

#include <Box2D/Box2D.h>

class Ball{
	private:
		b2Vec2 position;
		b2Vec2 velocity;
		b2Vec2 acceleration;

	public:

		Ball(b2Vec2 position = b2Vec2(0, 0), b2Vec2 velocity = b2Vec2(0, 0), b2Vec2 acceleration = b2Vec2(0, 0)) : position(position), velocity(velocity), acceleration(acceleration){
		}

		/**
		 * Retrieves the current ball position
		 * @return			b2Vec2		The current ball position
		 */
		b2Vec2 getPosition(){
			return this->position;
		}

		/**
		 * Retrieves the current ball velocity
		 * @return			b2Vec2		The current ball velocity
		 */
		b2Vec2 getVelocity(){
			return this->velocity;
		}

		/**
		 * Retrieves the current ball acceleration
		 * @return			b2Vec2		The current ball acceleration
		 */
		b2Vec2 getAcceleration(){
			return this->acceleration;
		}
};

#endif /* PINBALL_BOT_BALL */
