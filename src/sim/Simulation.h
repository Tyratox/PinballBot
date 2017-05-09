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

#include "../agent/State.h"

#include "ContactListener.h"
#include "UserData.h"

/**
 * Class used to simulate a pinball machine using Box2D
 */
class Simulation{

	/* Declare public constants first */
	public:

		static const int			VELOCITY_ITERATIONS;
		static const int			POSITION_ITERATIONS;
		static const int			PLAYINGFIELD_VERTEX_NUMBER;

		static const float			FIELD_WIDTH;
		static const float			FIELD_HEIGHT;
		static const float			FIELD_SLOPE;

		static const float			FIELD_CAPTURE_X_MIN;
		static const float			FIELD_CAPTURE_X_MAX;

		static const float			FIELD_CAPTURE_Y_MIN;
		static const float			FIELD_CAPTURE_Y_MAX;

		static const float			GRAVITY_X;
		static const float			GRAVITY_Y;

		static const float			BORDER_DENSITY;
		static const float			BORDER_FRICTION;
		static const float			BORDER_RESTITUTION;

		static const int			PIN_COUNT;
		static const float			PIN_RADIUS;
		static const float			PIN_DENSITY;
		static const float			PIN_FRICTION;
		static const float			PIN_RESTITUTION;

		static const float			PIN_BOUNDARY_X_MIN;
		static const float			PIN_BOUNDARY_X_MAX;

		static const float			PIN_BOUNDARY_Y_MIN;
		static const float			PIN_BOUNDARY_Y_MAX;

		static const float			KICKER_WIDTH;
		static const float			KICKER_HEIGHT;
		static const float			KICKER_DENSITY;
		static const float			KICKER_FRICTION;
		static const float			KICKER_RESTITUTION;

		static const float			KICKER_BORDER_POS_X;
		static const float			KICKER_BORDER_POS_Y;

		static const float			GAME_OVER_HEIGHT;
		static const float			GAME_OVER_WIDTH;

		static const float			BALL_WEIGHT;
		static const float			BALL_RADIUS;
		static const float			BALL_DENSITY;
		static const float			BALL_FRICTION;
		static const float			BALL_RESTITUTION;

		static const float			FLIPPER_WIDTH;
		static const float			FLIPPER_HEIGHT;
		static const float			FLIPPER_APEX_HEIGHT;
		static const float			FLIPPER_DENSITY;
		static const float			FLIPPER_FRICTION;
		static const float			FLIPPER_RESTITUTION;
		static const float			FLIPPER_REV_JOINT_LOWER_ANGLE;
		static const float			FLIPPER_REV_JOINT_UPPER_ANGLE;

		static const float			FLIPPER_REV_MOTOR_SPEED;
		static const float			FLIPPER_REV_MOTOR_MAX_TORQUE;

		static const float			FLIPPER_LEFT_POS_X;
		static const float			FLIPPER_RIGHT_POS_X;

		static const float			FLIPPER_LEFT_POS_Y;
		static const float			FLIPPER_RIGHT_POS_Y;

	/* Then some private things */
	private:

		ContactListener									contactListener;

		//The Box2D world where all the things take place
		b2Vec2											gravity;
		b2World											world;

		//The playing field
		b2Body*											playingFieldBody;

		//The pin bodies
		std::vector<b2Body*>							pinBodies;

		//Kicker border
		b2Body*											kickerBorderBody;

		//Kicker
		b2Body*											kickerBody;

		//The ball used to play the game
		b2Body*											ballBody;

		//Collision = gameover
		b2Body*											gameOverBody;

		//The flipper on the left hand side
		b2Body*											flipperLeftBody;
		b2RevoluteJoint*								flipperLeftRevJoint;

		//The flipper on the right hand side
		b2Body*											flipperRightBody;
		b2RevoluteJoint*								flipperRightRevJoint;

		//Gameover?
		bool											isGameOver;

		UserData										borderData;
		std::vector<UserData>							pinData;
		UserData										ballData;
		UserData										kickerData;
		UserData										gameOverData;
		UserData										flipperData;

		std::vector<b2Vec2>								staticPlayingField;

	/* And last but not least the public functions */
	public:

		//stores the current reward
		float											reward;

		/**
		 * Inits the world and all of the needed objects
		 */
		Simulation(bool randomKickerForce);

		/**
		 * Returns a reference to the Box2D world
		 */
		const b2World* getWorld();

		/**
		 * Draws the playing field
		 * @param	points		b2Vec2*		Array of points to connect by creating EdgeShapes
		 * @return				void
		 */
		void drawPlayingField(const b2Vec2* points);

		/**
		 * Respawns the ball
		 * @return void
		 */
		void respawnBall();

		/**
		 * (Re-)Generates the pin field
		 * @return void
		 */
		void generateRandomPinField();

		/**
		 * Generates a static pin field
		 */
		void generateStaticPinField();

		/**
		 * Function executed by the contact listener on game over
		 * @return void
		 */
		void gameOver();

		/**
		 * Function executed if a reward is received
		 * @return void
		 */

		void getReward(float reward = 0.0f);

		/**
		 * Steps a specific value forward in time
		 * @param	time_step	float32		The amount of time to step
		 * @return	void
		 */
		void step(const float32 &time_step);

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

		/**
		 * Returns if the playing ball is inside the capture frame
		 * @return bool
		 */
		bool isPlayingBallInsideCaptureFrame();

		/**
		 * Returns the current state
		 * @param availableActions	std::vector<Action*>	The actions for which the deault reward should be set
		 * @param includeVelocity	bool					Whether the velocity should be empty (false) or not (true)
		 * @return State
		 */
		State getCurrentState(std::vector<Action*> availableActions = std::vector<Action*>(0), bool includeVelocity = true);

};


#endif /* SIM_SIMULATION_H_ */
