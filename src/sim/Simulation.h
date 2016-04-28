/*
 * Simulation.h
 *
 *  The box2d simulation of the pinball machine
 */

#ifndef SIM_SIMULATION_H_
#define SIM_SIMULATION_H_

#include <Box2D/Box2D.h>

#include <vector>
#include <cmath>

#include "../agent/Ball.h"
#include "../agent/State.h"

#include "ContactListener.h"
#include "UserData.h"

/**
 * Class used to simulate a pinball machine using Box2D
 */
class Simulation{

	/* Declare public constants first */
	public:

		static const int32			VELOCITY_ITERATIONS;
		static const int32			POSITION_ITERATIONS;
		static const int32			PLAYINGFIELD_VERTEX_NUMBER;

		static const float			GRAVITY_X;
		static const float			GRAVITY_Y;

		static const float			FIELD_WIDTH;
		static const float			FIELD_HEIGHT;

		static const float			GAME_OVER_HEIGHT;
		static const float			GAME_OVER_WIDTH;

		static const float			BALL_RADIUS;
		static const float			BALL_DENSITY;
		static const float			BALL_FRICTION;
		static const float			BALL_RESTITUTION;

		static const float			FLIPPER_WIDTH;
		static const float			FLIPPER_HEIGHT;
		static const float			FLIPPER_DENSITY;
		static const float			FLIPPER_FRICTION;
		static const float			FLIPPER_RESTITUTION;
		static const float			FLIPPER_REV_JOINT_LOWER_ANGLE;
		static const float			FLIPPER_REV_JOINT_UPPER_ANGLE;

		static const float			FLIPPER_REV_MOTOR_SPEED;
		static const float			FLIPPER_REV_MOTOR_MAX_TORQUE;

	/* Then some private things */
	private:

		ContactListener									contactListener;

		//The Box2D world where all the things take place
		b2Vec2											gravity;
		b2World											world;

		//The playing field
		std::vector<b2Body*>							playingFieldBody;

		//The ball used to play the game
		b2Body*											ballBody;

		//Collision = gameover
		b2Body*											gameoverBody;

		//The flipper on the left hand side
		b2Body*											flipperLeftBody;
		b2RevoluteJoint*								flipperLeftRevJoint;

		//The flipper on the right hand side
		b2Body*											flipperRightBody;
		b2RevoluteJoint*								flipperRightRevJoint;

		UserData										borderData;
		UserData										ballData;
		UserData										gameOverData;
		UserData										flipperData;

	/* And last but not least the public functions */
	public:

		/**
		 * Inits the world and all of the needed objects
		 */
		Simulation();

		void drawPlayingField(const b2Vec2* points);

		/**
		 * Steps a specific value forward in time
		 * @param	time_step	float32		The amount of time to step
		 * @return	void
		 */
		void step(const float32 &time_step);

		/**
		 * Sets the DebugDraw renderer of the Box2D world
		 * @param	draw		b2Draw		A pointer to a class implementing the b2Draw functions
		 * @return	void
		 */
		void setRenderer(b2Draw* draw);

		/**
		 * Renders the scene by calling the DrawDebugData function inside the Box2D world,
		 * which calls back to the initially set b2Draw class
		 * @return	void
		 */
		void render();

		/**
		 * Activates the left hand flipper. Will stay active until disableLeftFlipper() is called
		 * @return	void
		 */
		void enableLeftFlipper();

		/**
		 * Deactivates the left hand flipper. Gravity will do it's job again
		 * @return	void
		 */
		void disableLeftFlipper();

		/**
		 * Activates the right hand flipper. Will stay active until disableLeftFlipper() is called
		 * @return	void
		 */
		void enableRightFlipper();
		/**
		 * Deactivates the right hand flipper. Gravity will do it's job again
		 * @return	void
		 */
		void disableRightFlipper();

		/**
		 * Prints debugging information about the playing ball
		 * @return	void
		 */
		void debugPlayingBall();

		State getCurrentState();

};


#endif /* SIM_SIMULATION_H_ */
