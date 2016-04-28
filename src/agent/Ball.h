/*
 * Ball.h
 *
 * Stores information about a ball
 */

#ifndef AGENT_BALL_H_
#define AGENT_BALL_H_


#include <Box2D/Box2D.h>

class Ball{
	private:
		b2Vec2 position;
		b2Vec2 velocity;

	public:

		Ball(b2Vec2 position = b2Vec2(0, 0), b2Vec2 velocity = b2Vec2(0, 0));

		/**
		 * Retrieves the current ball position
		 * @return			b2Vec2		The current ball position
		 */
		b2Vec2 getPosition();

		/**
		 * Sets the ball position
		 * @param	position	b2Vec2		The ball position to set
		 * @return				void
		 */
		void setPosition(b2Vec2 position);

		/**
		 * Retrieves the current ball velocity
		 * @return			b2Vec2		The current ball velocity
		 */
		b2Vec2 getVelocity();

		/**
		 * Sets the ball velocity
		 * @param	velocity	b2Vec2		The ball velocity to set
		 * @return				void
		 */
		void setVelocity(b2Vec2 velocity);
};


#endif /* AGENT_BALL_H_ */
