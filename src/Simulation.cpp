/*
 * Simulation.cpp
 *
 *  The box2d simulation of the pinball machine
 */

#ifndef PINBALL_BOT_SIMULATION
#define PINBALL_BOT_SIMULATION

#include <stdio.h>
#include <Box2D/Box2D.h>

#include "Ball.cpp"
#include "State.cpp"

const int32			VELOCITY_ITERATIONS					= 6;
const int32			POSITION_ITERATIONS					= 2;

const float			GRAVITY_X							= 0;
const float			GRAVITY_Y							= 5.0f; /* positive, cause we start in the top left corner */

const float			FLIPPER_HEIGHT						= 1.0f;
const float			FLIPPER_WIDTH						= 0.5f;

const float			WALL_THICKNESS						= 0.05f;

const float			BALL_RADIUS							= 0.025f;
const float			BALL_DENSITY						= 0.0001f;
const float			BALL_FRICTION						= 0.01f;
const float			BALL_RESTITUTION					= 0.9f;

const float			FLIPPER_DENSITY						= 0.0001f;
const float			FLIPPER_FRICTION					= 0.01f;
const float			FLIPPER_RESTITUTION					= 0.3f;
const float			FLIPPER_REV_JOINT_LOWER_ANGLE		= (float) 0.0f * b2_pi;
const float			FLIPPER_REV_JOINT_UPPER_ANGLE		= (float) 0.1f * b2_pi;

const float			FLIPPER_REV_MOTOR_SPEED				= (float) 1.5 * b2_pi; /* rad^-1 */
const float			FLIPPER_REV_MOTOR_MAX_TORQUE		= 5.0f;

/**
 * Class used to simulate a pinball machine using Box2D
 */
class PinballSimulation{
	private:

		//The Box2D world where all the things take place
		b2Vec2											gravity;
		b2World											world;

		//The ceiling
		b2BodyDef										ceilingBodyDef;
		b2Body*											ceilingBody;
		b2PolygonShape									ceilingBox;

		//The floor
		b2BodyDef										floorBodyDef;
		b2Body*											floorBody;
		b2PolygonShape									floorBox;

		//The left wall
		b2BodyDef										leftWallBodyDef;
		b2Body*											leftWallBody;
		b2PolygonShape									leftWallBox;

		//The right wall
		b2BodyDef										rightWallBodyDef;
		b2Body*											rightWallBody;
		b2PolygonShape									rightWallBox;

		//The ball used to play the game
		b2BodyDef										ballDef;
		b2Body*											ballBody;
		b2CircleShape									ballSphere;
		b2FixtureDef									ballFixtureDef;

		//The flipper on the left hand side
		b2BodyDef										flipperLeftDef;
		b2Body*											flipperLeftBody;
		b2PolygonShape									flipperLeftTriangle;
		b2FixtureDef									flipperLeftFixtureDef;
		b2RevoluteJointDef								flipperLeftRevJointDef;
		b2RevoluteJoint*								flipperLeftRevJoint;

		//The flipper on the right hand side
		b2BodyDef										flipperRightDef;
		b2Body*											flipperRightBody;
		b2PolygonShape									flipperRightTriangle;
		b2FixtureDef									flipperRightFixtureDef;
		b2RevoluteJointDef								flipperRightRevJointDef;
		b2RevoluteJoint*								flipperRightRevJoint;

	public:

		/**
		 * Inits the world and all of the needed objects
		 */
		PinballSimulation() : gravity(GRAVITY_X, GRAVITY_Y), world(this->gravity){
			/* Initializes a world with gravity pulling downwards */

			/* Sets the position of the boundaries. Remember: The origin (0|0) is the top left corner! */
			this->ceilingBodyDef.position.Set((FLIPPER_WIDTH/2), (WALL_THICKNESS/2));
			this->floorBodyDef.position.Set((FLIPPER_WIDTH/2), (FLIPPER_HEIGHT-(WALL_THICKNESS/2)));

			this->leftWallBodyDef.position.Set((WALL_THICKNESS/2), (FLIPPER_HEIGHT/2));
			this->rightWallBodyDef.position.Set((FLIPPER_WIDTH - (WALL_THICKNESS/2)),  (FLIPPER_HEIGHT/2));

			/* Adds the boundaries to the world */
			this->ceilingBody 							= world.CreateBody(&this->ceilingBodyDef);
			this->floorBody 							= world.CreateBody(&this->floorBodyDef);

			this->leftWallBody 							= world.CreateBody(&this->leftWallBodyDef);
			this->rightWallBody 						= world.CreateBody(&this->rightWallBodyDef);

			/* Define the box size of the boundaries, the entered values are half the size in the world */
			this->ceilingBox.SetAsBox(FLIPPER_WIDTH/2, WALL_THICKNESS/2);
			this->floorBox.SetAsBox(FLIPPER_WIDTH/2, WALL_THICKNESS/2);

			this->leftWallBox.SetAsBox(WALL_THICKNESS/2, FLIPPER_HEIGHT/2);
			this->rightWallBox.SetAsBox(WALL_THICKNESS/2, FLIPPER_HEIGHT/2);

			/* Creates fixtures for the boundaries, set the density to 0 because they're static anyway */
			ceilingBody->CreateFixture(&this->ceilingBox, 0.0f);
			floorBody->CreateFixture(&this->floorBox, 0.0f);

			leftWallBody->CreateFixture(&this->leftWallBox, 0.0f);
			rightWallBody->CreateFixture(&this->rightWallBox, 0.0f);

			/* Add 2 flippers */
			flipperLeftDef.type							= b2_dynamicBody;
			flipperRightDef.type						= b2_dynamicBody;

			flipperLeftDef.position.Set(WALL_THICKNESS, (3*FLIPPER_HEIGHT/4));
			flipperRightDef.position.Set(FLIPPER_WIDTH-WALL_THICKNESS, (3*FLIPPER_HEIGHT/4));

			/* Triangle */
			b2Vec2 leftFlipperVertices[3];
			leftFlipperVertices[0].Set(0.0f, 0.0f);
			leftFlipperVertices[1].Set(0.0f, 0.05f);
			leftFlipperVertices[2].Set(0.15f, 0.05f);

			flipperLeftTriangle.Set(leftFlipperVertices, 3);

			/* Triangle */
			b2Vec2 rightFlipperVertices[3];
			rightFlipperVertices[0].Set(0.0f, 0.0f);
			rightFlipperVertices[1].Set(0.0f, 0.05f);
			rightFlipperVertices[2].Set(-0.15f, 0.05f);

			flipperRightTriangle.Set(rightFlipperVertices, 3);

			flipperLeftBody								= world.CreateBody(&this->flipperLeftDef);
			flipperRightBody							= world.CreateBody(&this->flipperRightDef);

			this->flipperLeftFixtureDef.shape			= &this->flipperLeftTriangle;
			this->flipperRightFixtureDef.shape			= &this->flipperRightTriangle;

			this->flipperLeftFixtureDef.density			= this->flipperRightFixtureDef.density = FLIPPER_DENSITY;
			this->flipperLeftFixtureDef.friction		= this->flipperRightFixtureDef.friction = FLIPPER_FRICTION;
			this->flipperLeftFixtureDef.restitution		= this->flipperRightFixtureDef.restitution = FLIPPER_RESTITUTION;

			this->flipperLeftBody->CreateFixture(&this->flipperLeftFixtureDef);
			this->flipperRightBody->CreateFixture(&this->flipperRightFixtureDef);

			/* Connect the flippers to the walls with a joint */
			flipperLeftRevJointDef.bodyA				= leftWallBody;
			flipperLeftRevJointDef.bodyB				= flipperLeftBody;

			flipperRightRevJointDef.bodyA				= rightWallBody;
			flipperRightRevJointDef.bodyB				= flipperRightBody;

			flipperLeftRevJointDef.localAnchorA			= b2Vec2((WALL_THICKNESS/2), 0.0f);
			flipperLeftRevJointDef.localAnchorB			= b2Vec2(0.0f, 0.0f);

			flipperRightRevJointDef.localAnchorA		= b2Vec2((-WALL_THICKNESS/2), 0.0f);
			flipperRightRevJointDef.localAnchorB		= b2Vec2(0.0f, 0.0f);

			flipperLeftRevJointDef.collideConnected		= flipperRightRevJointDef.collideConnected		= false;

			flipperLeftRevJointDef.lowerAngle			= -1 * FLIPPER_REV_JOINT_UPPER_ANGLE;
			flipperLeftRevJointDef.upperAngle			= FLIPPER_REV_JOINT_LOWER_ANGLE;

			flipperRightRevJointDef.lowerAngle			= FLIPPER_REV_JOINT_LOWER_ANGLE;
			flipperRightRevJointDef.upperAngle			= FLIPPER_REV_JOINT_UPPER_ANGLE;

			flipperLeftRevJointDef.enableLimit			= flipperRightRevJointDef.enableLimit				= true;

			flipperLeftRevJointDef.maxMotorTorque		= flipperRightRevJointDef.maxMotorTorque			= FLIPPER_REV_MOTOR_MAX_TORQUE;
			flipperLeftRevJointDef.enableMotor			= flipperRightRevJointDef.enableMotor				= false; /* Not enabled by default */

			flipperLeftRevJointDef.motorSpeed 			= -1 * FLIPPER_REV_MOTOR_SPEED;
			flipperRightRevJointDef.motorSpeed 			= FLIPPER_REV_MOTOR_SPEED;

			flipperLeftRevJoint							= (b2RevoluteJoint*)this->world.CreateJoint(&flipperLeftRevJointDef);
			flipperRightRevJoint						= (b2RevoluteJoint*)this->world.CreateJoint(&flipperRightRevJointDef);

			/* Init playing ball */
			ballDef.type								= b2_dynamicBody;
			ballDef.position.Set((FLIPPER_WIDTH/2), (FLIPPER_HEIGHT/2));
			ballBody									= world.CreateBody(&this->ballDef);

			this->ballSphere.m_p.Set(0.0f, 0.0f);
			this->ballSphere.m_radius					= BALL_RADIUS;

			this->ballFixtureDef.shape					= &this->ballSphere;
			this->ballFixtureDef.density				= BALL_DENSITY;
			this->ballFixtureDef.friction				= BALL_FRICTION;
			this->ballFixtureDef.restitution			= BALL_RESTITUTION;

			this->ballBody->CreateFixture(&this->ballFixtureDef);
		}

		/**
		 * Steps a specific value forward in time
		 * @param	time_step	float32		The amount of time to step
		 * @return	void
		 */
		void step(const float32 &time_step){
			world.Step(time_step, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
		}

		/**
		 * Sets the DebugDraw renderer of the Box2D world
		 * @param	draw		b2Draw		A pointer to a class implementing the b2Draw functions
		 * @return	void
		 */
		void setRenderer(b2Draw* draw){
			this->world.SetDebugDraw( draw );
		}

		/**
		 * Renders the scene by calling the DrawDebugData function inside the Box2D world,
		 * which calls back to the initially set b2Draw class
		 * @return	void
		 */
		void render(){
			this->world.DrawDebugData();
		}

		/**
		 * Activates the left hand flipper. Will stay active until disableLeftFlipper() is called
		 * @return	void
		 */
		void enableLeftFlipper(){
			flipperLeftRevJoint->EnableMotor(true);
		}
		/**
		 * Deactivates the left hand flipper. Gravity will do it's job again
		 * @return	void
		 */
		void disableLeftFlipper(){
			flipperLeftRevJoint->EnableMotor(false);
		}

		/**
		 * Activates the right hand flipper. Will stay active until disableLeftFlipper() is called
		 * @return	void
		 */
		void enableRightFlipper(){
			flipperRightRevJoint->EnableMotor(true);
		}
		/**
		 * Deactivates the right hand flipper. Gravity will do it's job again
		 * @return	void
		 */
		void disableRightFlipper(){
			flipperRightRevJoint->EnableMotor(false);
		}

		/**
		 * Prints debugging information about the playing ball
		 * @return	void
		 */
		void debugPlayingBall(){
			b2Vec2 position								= this->ballBody->GetPosition();
			float32 angle								= this->ballBody->GetAngle();

			printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
		}

		State getCurrentState(){
			return State(Ball(this->ballBody->GetPosition(), this->ballBody->GetLinearVelocity()));
		}

		class s{

		};

};

#endif /* PINBALL_BOT_SIMULATION */
