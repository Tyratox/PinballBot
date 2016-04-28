/*
 * Simulation.cpp
 *
 *  The box2d simulation of the pinball machine
 */

#include <Box2D/Box2D.h>

#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <functional>

#include "../agent/Ball.h"
#include "../agent/State.h"

#include "Simulation.h"
#include "ContactListener.h"
#include "UserData.h"

const int32			Simulation::VELOCITY_ITERATIONS					= 6;
const int32			Simulation::POSITION_ITERATIONS					= 2;
const int32			Simulation::PLAYINGFIELD_VERTEX_NUMBER			= 29;

const float			Simulation::FIELD_WIDTH							= 0.5f;
const float			Simulation::FIELD_HEIGHT						= 1.0f;
const float			Simulation::FIELD_SLOPE							= b2_pi / 6; //30 deg

const float			Simulation::GRAVITY_X							= 0;
const float			Simulation::GRAVITY_Y							= std::sin(FIELD_SLOPE) * 9.81f; /* positive, cause we start in the top left corner */

const float			Simulation::BORDER_DENSITY						= 0.0f;
const float			Simulation::BORDER_FRICTION						= 1.0f;
const float			Simulation::BORDER_RESTITUTION					= 0.01f;

const float			Simulation::KICKER_WIDTH						= FIELD_WIDTH / 12;
const float			Simulation::KICKER_HEIGHT						= 0.01;
const float			Simulation::KICKER_DENSITY						= 0.0f;
const float			Simulation::KICKER_FRICTION						= 1.0f;
const float			Simulation::KICKER_RESTITUTION					= 0.3f;

const float			Simulation::GAME_OVER_WIDTH						= (2 * FIELD_HEIGHT/8);
const float			Simulation::GAME_OVER_HEIGHT					= 0.01f;

const float			Simulation::BALL_WEIGHT							= 0.08f; //80 g
const float			Simulation::BALL_RADIUS							= 0.025f; // 25 mm
const float			Simulation::BALL_DENSITY						= (BALL_RADIUS*BALL_RADIUS*b2_pi)/BALL_WEIGHT;
const float			Simulation::BALL_FRICTION						= 0.01f;
const float			Simulation::BALL_RESTITUTION					= 0.5f;

const float			Simulation::FLIPPER_HEIGHT						= 0.05f;
const float			Simulation::FLIPPER_WIDTH						= 0.085f;
const float			Simulation::FLIPPER_APEX_HEIGHT					= 0.03f;
const float			Simulation::FLIPPER_DENSITY						= 100.0f;
const float			Simulation::FLIPPER_FRICTION					= 0.5f;
const float			Simulation::FLIPPER_RESTITUTION					= 0.75f;
const float			Simulation::FLIPPER_REV_JOINT_LOWER_ANGLE		= (float) 0.0f * b2_pi;
const float			Simulation::FLIPPER_REV_JOINT_UPPER_ANGLE		= (float) 0.2f * b2_pi;

const float			Simulation::FLIPPER_REV_MOTOR_SPEED				= (float) 10 * b2_pi; /* rad^-1 */
const float			Simulation::FLIPPER_REV_MOTOR_MAX_TORQUE		= 5.0f;

Simulation::Simulation():
	contactListener(std::bind(&Simulation::gameOver, this)),
	gravity(GRAVITY_X, GRAVITY_Y),
	world(this->gravity),
	playingFieldBody(PLAYINGFIELD_VERTEX_NUMBER),
	ballBody(NULL),
	gameOverBody(NULL),
	flipperLeftBody(NULL),
	flipperRightBody(NULL),
	borderData(UserData::PINBALL_BORDER),
	ballData(UserData::PINBALL_BALL),
	kickerData(UserData::PINBALL_KICKER),
	gameOverData(UserData::PINBALL_GAMEOVER),
	flipperData(UserData::PINBALL_FLIPPER){

	/* Initializes a world with gravity pulling downwards and add contact listener */
	world.SetContactListener(&contactListener);

	/* Remember: The origin (0|0) is the top left corner! */

	/* Define edge shape of the playing field */
	b2Vec2 playingFieldVertices[PLAYINGFIELD_VERTEX_NUMBER];
	playingFieldVertices[0].Set(FIELD_HEIGHT / 8, 0);
	playingFieldVertices[1].Set((FIELD_HEIGHT / 8) - (std::sin(1 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(1 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[2].Set((FIELD_HEIGHT / 8) - (std::sin(2 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(2 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[3].Set((FIELD_HEIGHT / 8) - (std::sin(3 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(3 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[4].Set((FIELD_HEIGHT / 8) - (std::sin(4 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(4 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[5].Set((FIELD_HEIGHT / 8) - (std::sin(5 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(5 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[6].Set((FIELD_HEIGHT / 8) - (std::sin(6 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(6 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[7].Set((FIELD_HEIGHT / 8) - (std::sin(7 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(7 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[8].Set((FIELD_HEIGHT / 8) - (std::sin(8 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(8 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[9].Set((FIELD_HEIGHT / 8) - (std::sin(9 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::cos(9 * b2_pi / 20)*FIELD_HEIGHT / 8));

	playingFieldVertices[10].Set(		0				, 6 *	FIELD_HEIGHT / 8);
	playingFieldVertices[11].Set(		FIELD_WIDTH / 6	, 7 *	FIELD_HEIGHT / 8);
	playingFieldVertices[12].Set(		FIELD_WIDTH / 6	,		FIELD_HEIGHT);
	playingFieldVertices[13].Set(4 *	FIELD_WIDTH / 6	,		FIELD_HEIGHT);
	playingFieldVertices[14].Set(4 *	FIELD_WIDTH / 6	, 7 *	FIELD_HEIGHT / 8);
	playingFieldVertices[15].Set(5 *	FIELD_WIDTH / 6	, 6 *	FIELD_HEIGHT / 8);
	playingFieldVertices[16].Set(6 *	FIELD_WIDTH / 6	, 6 *	FIELD_HEIGHT / 8);
	playingFieldVertices[17].Set(6 *	FIELD_WIDTH / 6	, 1 *	FIELD_HEIGHT / 8);

	playingFieldVertices[18].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(1 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(1 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[19].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(2 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(2 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[20].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(3 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(3 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[21].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(4 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(4 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[22].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(5 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(5 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[23].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(6 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(6 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[24].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(7 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(7 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[25].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(8 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(8 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[26].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 + (std::cos(9 * b2_pi / 20)*FIELD_HEIGHT / 8), (FIELD_HEIGHT / 8) - (std::sin(9 * b2_pi / 20)*FIELD_HEIGHT / 8));
	playingFieldVertices[27].Set(FIELD_WIDTH - FIELD_HEIGHT / 8 ,0);
	playingFieldVertices[28].Set(FIELD_HEIGHT / 8			, 0);

	drawPlayingField(playingFieldVertices);

	/* kicker Border */
	b2BodyDef									kickerBorderDef;
	kickerBorderDef.type						= b2_staticBody;
	kickerBorderDef.position.Set(5 * FIELD_WIDTH / 6, 6 * FIELD_HEIGHT / 8);

	kickerBorderBody							= world.CreateBody(&kickerBorderDef);

	kickerBorderBody->SetUserData(&borderData);
	
	b2EdgeShape									kickerBorderShape;
	kickerBorderShape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, -1 * 4 * FIELD_HEIGHT / 8));

	b2FixtureDef								kickerBorderFixtureDef;
	kickerBorderFixtureDef.shape				= &kickerBorderShape;
	kickerBorderFixtureDef.density				= BORDER_DENSITY;
	kickerBorderFixtureDef.friction				= BORDER_FRICTION;
	kickerBorderFixtureDef.restitution			= BORDER_RESTITUTION;
	kickerBorderBody->CreateFixture(&kickerBorderFixtureDef);

	/* Add the kicker itself */
	b2BodyDef									kickerDef;
	kickerDef.type								= b2_staticBody;
	kickerDef.position.Set(11 * FIELD_WIDTH / 12, (6 * FIELD_HEIGHT / 8) - (KICKER_HEIGHT/2));

	kickerBody								= world.CreateBody(&kickerDef);

	kickerBody->SetUserData(&kickerData);

	b2PolygonShape kickerBox;
	kickerBox.SetAsBox(KICKER_WIDTH/2, KICKER_HEIGHT/2);

	b2FixtureDef								kickerFixtureDef;
	kickerFixtureDef.shape						= &kickerBox;
	kickerFixtureDef.density					= KICKER_DENSITY;
	kickerFixtureDef.friction					= KICKER_FRICTION;
	kickerFixtureDef.restitution				= KICKER_RESTITUTION;
	kickerBody->CreateFixture(&kickerFixtureDef);

	/* Add the game over field */
	b2BodyDef									gameOverDef;
	gameOverDef.type							= b2_staticBody;
	gameOverDef.position.Set(5 * FIELD_WIDTH / 12, FIELD_HEIGHT - (GAME_OVER_HEIGHT/2));

	gameOverBody								= world.CreateBody(&gameOverDef);

	gameOverBody->SetUserData(&gameOverData);

	b2PolygonShape gameOverBox;
	gameOverBox.SetAsBox(GAME_OVER_WIDTH/2, GAME_OVER_HEIGHT/2);
	gameOverBody->CreateFixture(&gameOverBox, 0.0f);

	/* Add the two flippers */
	b2BodyDef									flipperLeftDef;
	b2BodyDef									flipperRightDef;

	flipperLeftDef.type							= b2_dynamicBody;
	flipperRightDef.type						= b2_dynamicBody;

	flipperLeftDef.position.Set((FIELD_HEIGHT/8), (7*FIELD_HEIGHT/8));
	flipperRightDef.position.Set((3*FIELD_HEIGHT/8), (7*FIELD_HEIGHT/8));

	flipperLeftBody								= world.CreateBody(&flipperLeftDef);
	flipperRightBody							= world.CreateBody(&flipperRightDef);

	flipperLeftBody->SetUserData(&flipperData);
	flipperRightBody->SetUserData(&flipperData);

	b2FixtureDef								flipperLeftFixtureDef;
	b2FixtureDef								flipperRightFixtureDef;

	b2PolygonShape								flipperRightTriangle;
	b2PolygonShape								flipperLeftTriangle;

	/* Triangle */
	b2Vec2 leftFlipperVertices[3];
	leftFlipperVertices[0].Set(0.0f, 0.0f);
	leftFlipperVertices[1].Set(0.0f, FLIPPER_HEIGHT);
	leftFlipperVertices[2].Set(FLIPPER_WIDTH, FLIPPER_APEX_HEIGHT);

	flipperLeftTriangle.Set(leftFlipperVertices, 3);

	/* Triangle */
	b2Vec2 rightFlipperVertices[3];
	rightFlipperVertices[0].Set(0.0f, 0.0f);
	rightFlipperVertices[1].Set(0.0f, FLIPPER_HEIGHT);
	rightFlipperVertices[2].Set(-FLIPPER_WIDTH, FLIPPER_APEX_HEIGHT);

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

	flipperLeftRevJointDef.bodyA				= playingFieldBody[11];
	flipperLeftRevJointDef.bodyB				= flipperLeftBody;

	flipperRightRevJointDef.bodyA				= playingFieldBody[14];
	flipperRightRevJointDef.bodyB				= flipperRightBody;

	flipperLeftRevJointDef.localAnchorA			= b2Vec2(0.0f, 0.0f);
	flipperLeftRevJointDef.localAnchorB			= b2Vec2(0.0f, 0.0f);

	flipperRightRevJointDef.localAnchorA		= b2Vec2(0.0f, 0.0f);
	flipperRightRevJointDef.localAnchorB		= b2Vec2(0.0f, 0.0f);

	flipperLeftRevJointDef.collideConnected		= flipperRightRevJointDef.collideConnected			= false;

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

	respawnBall();
}

void Simulation::drawPlayingField(const b2Vec2* points){
	for(int i=0;i<PLAYINGFIELD_VERTEX_NUMBER-1;i++){

		/*
		 * The world object does not keep a reference to the body definition.
		 */
		b2BodyDef	def;
		def.type					=	b2_staticBody;
		def.position.Set(points[i].x, points[i].y);

		playingFieldBody[i]				= world.CreateBody(&def);

		playingFieldBody[i]->SetUserData(&borderData);

		/*
		 * Factories do not retain references to the definitions.
		 * So you can create definitions on the stack and keep them in temporary resources.
		 */
		b2EdgeShape edge;
		edge.Set(b2Vec2(0.0f, 0.0f), points[i+1]-points[i]);

		b2FixtureDef								borderFixtureDef;
		borderFixtureDef.shape						= &edge;
		borderFixtureDef.density					= BORDER_DENSITY;
		borderFixtureDef.friction					= BORDER_FRICTION;
		borderFixtureDef.restitution				= BORDER_RESTITUTION;

		playingFieldBody[i]->CreateFixture(&borderFixtureDef);

	}
}

void Simulation::respawnBall(){

	if(ballBody){
		//if not a null pointer
		world.DestroyBody(ballBody);
	}

	/* Init playing ball */
	b2BodyDef									ballDef;
	ballDef.type								= b2_dynamicBody;
	ballDef.position.Set(11 * FIELD_WIDTH / 12, 4 * FIELD_HEIGHT / 8);
	ballBody									= world.CreateBody(&ballDef);

	ballBody->SetUserData(&ballData);

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

void Simulation::gameOver(){
	isGameOver = true;
}

void Simulation::step(const float32 &time_step){
	if(isGameOver){
		respawnBall();
		isGameOver = false;
	}
	world.Step(time_step, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

void Simulation::setRenderer(b2Draw* draw){
	this->world.SetDebugDraw( draw );
}

void Simulation::render(){
	this->world.DrawDebugData();
}

void Simulation::enableLeftFlipper(){
	flipperLeftRevJoint->EnableMotor(true);
}

void Simulation::disableLeftFlipper(){
	flipperLeftRevJoint->EnableMotor(false);
}

void Simulation::enableRightFlipper(){
	flipperRightRevJoint->EnableMotor(true);
}

void Simulation::disableRightFlipper(){
	flipperRightRevJoint->EnableMotor(false);
}

void Simulation::debugPlayingBall(){
	b2Vec2 position								= this->ballBody->GetPosition();
	float32 angle								= this->ballBody->GetAngle();

	printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}

State Simulation::getCurrentState(){
	return State(Ball(this->ballBody->GetPosition(), this->ballBody->GetLinearVelocity()), flipperLeftRevJoint->IsMotorEnabled(), flipperRightRevJoint->IsMotorEnabled());
}
