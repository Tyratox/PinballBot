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

	public:

		Ball(b2Vec2 position = b2Vec2(0, 0), b2Vec2 velocity = b2Vec2(0, 0)) : position(position), velocity(velocity){
		}

		/**
		 * Retrieves the current ball position
		 * @return			b2Vec2		The current ball position
		 */
		b2Vec2 getPosition(){
			return this->position;
		}

		/**
		 * Sets the ball position
		 * @param	position	b2Vec2		The ball position to set
		 * @return				void
		 */
		void setPosition(b2Vec2 position){
			this->position = position;
		}

		/**
		 * Retrieves the current ball velocity
		 * @return			b2Vec2		The current ball velocity
		 */
		b2Vec2 getVelocity(){
			return this->velocity;
		}

		/**
		 * Sets the ball velocity
		 * @param	velocity	b2Vec2		The ball velocity to set
		 * @return				void
		 */
		void setVelocity(b2Vec2 velocity){
			this->velocity = velocity;
		}

		/**
		 * Retrieves the current ball acceleration
		 * @return			b2Vec2		The current ball acceleration
		 */
		/*b2Vec2 getAcceleration(){
			return this->acceleration;
		}*/

		/**
		 * Sets the ball acceleration
		 * @param	acceleration	b2Vec2		The ball v to set
		 * @return					void
		 */
		/*void setAcceleration(b2Vec2 acceleration){
			this->acceleration = acceleration;
		}*/
};

#endif /* PINBALL_BOT_BALL */
