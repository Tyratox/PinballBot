/*
 * Simulation.cpp
 *
 *  The box2d simulation of the pinball machine
 */

#ifndef PINBALL_BOT_SIMULATION
#define PINBALL_BOT_SIMULATION

#include <stdio.h>
#include <Box2D/Box2D.h>

#include <cmath>

#include "../agent/Ball.cpp"
#include "../agent/State.cpp"

const int32			VELOCITY_ITERATIONS					= 6;
const int32			POSITION_ITERATIONS					= 2;
const int32			PLAYINGFIELD_VERTEX_NUMBER			= 10;

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

		//The playing field
		b2Body*											playingFieldBody[PLAYINGFIELD_VERTEX_NUMBER];

		//The ball used to play the game
		b2Body*											ballBody;

		//The flipper on the left hand side
		b2Body*											flipperLeftBody;
		b2RevoluteJoint*								flipperLeftRevJoint;

		//The flipper on the right hand side
		b2Body*											flipperRightBody;
		b2RevoluteJoint*								flipperRightRevJoint;

	public:

		/**
		 * Inits the world and all of the needed objects
		 */
		PinballSimulation() : gravity(GRAVITY_X, GRAVITY_Y), world(this->gravity){
			/* Initializes a world with gravity pulling downwards */

			/* Remember: The origin (0|0) is the top left corner! */

			/* Define edge shape of the playing field */
			b2Vec2 playingFieldVertices[PLAYINGFIELD_VERTEX_NUMBER];
			playingFieldVertices[0].Set(FLIPPER_HEIGHT / 8, 0);
			playingFieldVertices[1].Set((FLIPPER_HEIGHT / 8) - (std::sin(1 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(1 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			playingFieldVertices[2].Set((FLIPPER_HEIGHT / 8) - (std::sin(2 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(2 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			playingFieldVertices[3].Set((FLIPPER_HEIGHT / 8) - (std::sin(3 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(3 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			playingFieldVertices[4].Set((FLIPPER_HEIGHT / 8) - (std::sin(4 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(4 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			playingFieldVertices[5].Set((FLIPPER_HEIGHT / 8) - (std::sin(5 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(5 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			playingFieldVertices[6].Set((FLIPPER_HEIGHT / 8) - (std::sin(6 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(6 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			playingFieldVertices[7].Set((FLIPPER_HEIGHT / 8) - (std::sin(7 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(7 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			playingFieldVertices[8].Set((FLIPPER_HEIGHT / 8) - (std::sin(8 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(8 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			playingFieldVertices[9].Set((FLIPPER_HEIGHT / 8) - (std::sin(9 * b2_pi / 20)*FLIPPER_HEIGHT / 8), (FLIPPER_HEIGHT / 8) - (std::cos(9 * b2_pi / 20)*FLIPPER_HEIGHT / 8));
			

			drawPlayingField(playingFieldVertices);
			
			/* Add the two flippers */
			b2BodyDef									flipperLeftDef;
			b2BodyDef									flipperRightDef;

			flipperLeftDef.type							= b2_dynamicBody;
			flipperRightDef.type						= b2_dynamicBody;

			flipperLeftDef.position.Set(WALL_THICKNESS, (3*FLIPPER_HEIGHT/4));
			flipperRightDef.position.Set(FLIPPER_WIDTH-WALL_THICKNESS, (3*FLIPPER_HEIGHT/4));

			flipperLeftBody								= world.CreateBody(&flipperLeftDef);
			flipperRightBody							= world.CreateBody(&flipperRightDef);

			b2FixtureDef								flipperLeftFixtureDef;
			b2FixtureDef								flipperRightFixtureDef;

			b2PolygonShape								flipperRightTriangle;
			b2PolygonShape								flipperLeftTriangle;

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

			flipperLeftFixtureDef.shape					= &flipperLeftTriangle;
			flipperRightFixtureDef.shape				= &flipperRightTriangle;

			flipperLeftFixtureDef.density				= flipperRightFixtureDef.density		= FLIPPER_DENSITY;
			flipperLeftFixtureDef.friction				= flipperRightFixtureDef.friction		= FLIPPER_FRICTION;
			flipperLeftFixtureDef.restitution			= flipperRightFixtureDef.restitution	= FLIPPER_RESTITUTION;

			this->flipperLeftBody->CreateFixture(&flipperLeftFixtureDef);
			this->flipperRightBody->CreateFixture(&flipperRightFixtureDef);

			/* Connect the flippers to the walls with a joint */
			b2RevoluteJointDef							flipperLeftRevJointDef;
			b2RevoluteJointDef							flipperRightRevJointDef;

			flipperLeftRevJointDef.bodyA				= playingFieldBody[0];
			flipperLeftRevJointDef.bodyB				= flipperLeftBody;

			flipperRightRevJointDef.bodyA				= playingFieldBody[0];
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
			flipperLeftRevJointDef.enableMotor			= flipperRightRevJointDef.enableMotor				= false; // Not enabled by default

			flipperLeftRevJointDef.motorSpeed 			= -1 * FLIPPER_REV_MOTOR_SPEED;
			flipperRightRevJointDef.motorSpeed 			= FLIPPER_REV_MOTOR_SPEED;

			flipperLeftRevJoint							= (b2RevoluteJoint*)this->world.CreateJoint(&flipperLeftRevJointDef);
			flipperRightRevJoint						= (b2RevoluteJoint*)this->world.CreateJoint(&flipperRightRevJointDef);

			/* Init playing ball */
			b2BodyDef									ballDef;
			ballDef.type								= b2_dynamicBody;
			ballDef.position.Set((FLIPPER_WIDTH/2), (FLIPPER_HEIGHT/2));
			ballBody									= world.CreateBody(&ballDef);

			b2CircleShape								ballSphere;
			ballSphere.m_p.Set(0.0f, 0.0f);
			ballSphere.m_radius							= BALL_RADIUS;

			b2FixtureDef								ballFixtureDef;
			ballFixtureDef.shape						= &ballSphere;
			ballFixtureDef.density						= BALL_DENSITY;
			ballFixtureDef.friction						= BALL_FRICTION;
			ballFixtureDef.restitution					= BALL_RESTITUTION;

			this->ballBody->CreateFixture(&ballFixtureDef);
		}

		void drawPlayingField(const b2Vec2* points){
			for(int i=0;i<PLAYINGFIELD_VERTEX_NUMBER-1;i++){

				/*
				 * The world object does not keep a reference to the body definition.
				 */
				b2BodyDef	def;
				def.type					=	b2_staticBody;
				def.position.Set(0, 0);

				playingFieldBody[i]				= world.CreateBody(&def);

				/*
				 * Factories do not retain references to the definitions.
				 * So you can create definitions on the stack and keep them in temporary resources.
				 */
				b2EdgeShape edge;
				edge.Set(points[i], points[i+1]);

				playingFieldBody[i]->CreateFixture(&edge, 0.0f);

			}
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
			return State(Ball(this->ballBody->GetPosition(), this->ballBody->GetLinearVelocity()), flipperLeftRevJoint->IsMotorEnabled(), flipperRightRevJoint->IsMotorEnabled());
		}

		class s{

		};

};

#endif /* PINBALL_BOT_SIMULATION */
