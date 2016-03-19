/*
 * Simulation.cpp
 *
 *  The box2d simulation of the pinball machine
 */

#include <stdio.h>
#include <Box2D/Box2D.h>

const float32		TIME_STEP				= 1.0f / 60.0f;
const int32			VELOCITY_ITERATIONS		= 6;
const int32			POSITION_ITERATIONS		= 2;

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

		PinballSimulation() : gravity(0.0f, -5.0f), world(this->gravity){
			/* Initializes a world with gravity pulling downwards */

			/* Sets the position of the 4 boundaries */
			this->ceilingBodyDef.position.Set(0.0f, 2.0f);
			this->floorBodyDef.position.Set(0.0f, -2.0f);

			this->leftWallBodyDef.position.Set(-2.0f, 0.0f);
			this->rightWallBodyDef.position.Set(2.0f,  0.0f);

			/* Adds the 4 boundaries to the world */
			this->ceilingBody 					= world.CreateBody(&this->ceilingBodyDef);
			this->floorBody 					= world.CreateBody(&this->floorBodyDef);

			this->leftWallBody 					= world.CreateBody(&this->leftWallBodyDef);
			this->rightWallBody 				= world.CreateBody(&this->rightWallBodyDef);

			/* Define the box size of the boundaries, the entered values are half the size in the world */
			this->ceilingBox.SetAsBox(1.0f, 0.05f);
			this->floorBox.SetAsBox(1.0f, 0.05f);

			this->leftWallBox.SetAsBox(0.05f, 1.0f);
			this->rightWallBox.SetAsBox(0.05f, 1.0f);

			/* Creates fixtures for the boundaries, set the density to 0 because they're static anyway */
			ceilingBody->CreateFixture(&this->ceilingBox, 0.0f);
			floorBody->CreateFixture(&this->floorBox, 0.0f);

			leftWallBody->CreateFixture(&this->leftWallBox, 0.0f);
			rightWallBody->CreateFixture(&this->rightWallBox, 0.0f);

			/* Init playing ball */
			ballDef.type						= b2_dynamicBody;
			ballDef.position.Set(0.0f, 0.0f);
			ballBody							= world.CreateBody(&this->ballDef);

			this->ballSphere.m_p.Set(0.0f, 0.0f);
			this->ballSphere.m_radius			= 0.2f;

			this->ballFixtureDef.shape			= &this->ballSphere;
			this->ballFixtureDef.density		= 1.0f;
			this->ballFixtureDef.friction		= 0.3f;

			this->ballBody->CreateFixture(&this->ballFixtureDef);
		}

		void step(){
			world.Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
		}

		void debugPlayingBall(){
			b2Vec2 position						= this->ballBody->GetPosition();
			float32 angle						= this->ballBody->GetAngle();

			printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
		}

};
