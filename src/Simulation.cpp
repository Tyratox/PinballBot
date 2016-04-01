/*
 * Simulation.cpp
 *
 *  The box2d simulation of the pinball machine
 */

#ifndef PINBALL_BOT_SIMULATION
#define PINBALL_BOT_SIMULATION

#include <stdio.h>
#include <Box2D/Box2D.h>

const int32			VELOCITY_ITERATIONS		= 6;
const int32			POSITION_ITERATIONS		= 2;

const float			GRAVITY_X				= 0;
const float			GRAVITY_Y				= 5.0f; /*positive, cause we start in the top left corner*/

const float			FLIPPER_HEIGHT			= 1.5f;
const float			FLIPPER_WIDTH			= 0.75f;

const float			WALL_THICKNESS			= 0.05f;

const float			BALL_RADIUS				= 0.05f;
const float			BALL_DENSITY			= 0.0001f;
const float			BALL_FRICTION			= 0.01f;
const float			BALL_RESTITUTION		= 1.0f;

class PinballSimulation{
	private:
		b2Vec2									gravity;
		b2World									world;

		b2BodyDef								ceilingBodyDef;
		b2Body*									ceilingBody;
		b2PolygonShape							ceilingBox;

		b2BodyDef								floorBodyDef;
		b2Body*									floorBody;
		b2PolygonShape							floorBox;


		b2BodyDef								leftWallBodyDef;
		b2Body*									leftWallBody;
		b2PolygonShape							leftWallBox;

		b2BodyDef								rightWallBodyDef;
		b2Body*									rightWallBody;
		b2PolygonShape							rightWallBox;

		b2BodyDef								ballDef;
		b2Body*									ballBody;
		b2CircleShape							ballSphere;
		b2FixtureDef							ballFixtureDef;

	public:

		PinballSimulation() : gravity(GRAVITY_X, GRAVITY_Y), world(this->gravity){
			/* Initializes a world with gravity pulling downwards */

			/* Sets the position of the 4 boundaries */
			this->ceilingBodyDef.position.Set((FLIPPER_WIDTH/2), (WALL_THICKNESS/2));
			this->floorBodyDef.position.Set((FLIPPER_WIDTH/2), (FLIPPER_HEIGHT-(WALL_THICKNESS/2)));

			this->leftWallBodyDef.position.Set((WALL_THICKNESS/2), (FLIPPER_HEIGHT/2));
			this->rightWallBodyDef.position.Set((FLIPPER_WIDTH - (WALL_THICKNESS/2)),  (FLIPPER_HEIGHT/2));

			/* Adds the 4 boundaries to the world */
			this->ceilingBody 					= world.CreateBody(&this->ceilingBodyDef);
			this->floorBody 					= world.CreateBody(&this->floorBodyDef);

			this->leftWallBody 					= world.CreateBody(&this->leftWallBodyDef);
			this->rightWallBody 				= world.CreateBody(&this->rightWallBodyDef);

			/* Define the box size of the boundaries, the entered values are half the size in the world */
			this->ceilingBox.SetAsBox(FLIPPER_WIDTH, WALL_THICKNESS);
			this->floorBox.SetAsBox(FLIPPER_WIDTH, WALL_THICKNESS);

			this->leftWallBox.SetAsBox(WALL_THICKNESS, FLIPPER_HEIGHT);
			this->rightWallBox.SetAsBox(WALL_THICKNESS, FLIPPER_HEIGHT);

			/* Creates fixtures for the boundaries, set the density to 0 because they're static anyway */
			ceilingBody->CreateFixture(&this->ceilingBox, 0.0f);
			floorBody->CreateFixture(&this->floorBox, 0.0f);

			leftWallBody->CreateFixture(&this->leftWallBox, 0.0f);
			rightWallBody->CreateFixture(&this->rightWallBox, 0.0f);

			/* Init playing ball */
			ballDef.type						= b2_dynamicBody;
			ballDef.position.Set((FLIPPER_WIDTH/2) - (BALL_RADIUS/2), (FLIPPER_HEIGHT/2) - (BALL_RADIUS/2));
			ballBody							= world.CreateBody(&this->ballDef);

			this->ballSphere.m_p.Set(0.0f, 0.0f);
			this->ballSphere.m_radius			= BALL_RADIUS;

			this->ballFixtureDef.shape			= &this->ballSphere;
			this->ballFixtureDef.density		= BALL_DENSITY;
			this->ballFixtureDef.friction		= BALL_FRICTION;
			this->ballFixtureDef.restitution	= BALL_RESTITUTION;

			this->ballBody->CreateFixture(&this->ballFixtureDef);
		}

		void step(float32 time_step){
			world.Step(time_step, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
		}

		void setRenderer(b2Draw* draw){
			this->world.SetDebugDraw( draw );
		}

		void render(){
			this->world.DrawDebugData();
		}

		void debugPlayingBall(){
			b2Vec2 position						= this->ballBody->GetPosition();
			float32 angle						= this->ballBody->GetAngle();

			printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
		}

};

#endif /* PINBALL_BOT_SIMULATION */
