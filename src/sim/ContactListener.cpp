/*
 * ListenerContactPoints.cpp
 *
 *  A contact listener for bodys
 */

#include <Box2D/Box2D.h>

#include <functional>

#include "ContactListener.h"
#include "UserData.h"
#include "Simulation.h"

const float ContactListener::KICKER_FORCE_X		= 0.0f;
const float ContactListener::KICKER_FORCE_Y		= 1.0f;

ContactListener::ContactListener(std::function<void(void)> gameOverCallback):
	gameOverCallback(gameOverCallback){}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){

	if(contact->IsEnabled()){
		UserData *userDataA = (UserData*) contact->GetFixtureA()->GetBody()->GetUserData();
		UserData *userDataB = (UserData*) contact->GetFixtureB()->GetBody()->GetUserData();

		b2Body *ball;
		b2Body *otherObject;

		UserData *ball_data;
		UserData *otherObject_data;

		if(userDataA->type == UserData::PINBALL_BALL){
			ball				= contact->GetFixtureA()->GetBody();
			ball_data			= userDataA;

			otherObject			= contact->GetFixtureB()->GetBody();
			otherObject_data	= userDataB;
		}else if(userDataB->type == UserData::PINBALL_BALL){
			ball				= contact->GetFixtureB()->GetBody();
			ball_data			= userDataB;

			otherObject			= contact->GetFixtureA()->GetBody();
			otherObject_data	= userDataA;
		}else if(userDataA->type == UserData::PINBALL_FLIPPER && userDataB->type == UserData::PINBALL_BORDER){
			contact->SetEnabled(false); //No collision between flippers and border
			return;
		}else if(userDataB->type == UserData::PINBALL_FLIPPER && userDataA->type == UserData::PINBALL_BORDER){
			contact->SetEnabled(false); //No collision between flippers and border
			return;
		}else if(userDataB->type == UserData::PINBALL_FLIPPER && userDataA->type == UserData::PINBALL_FLIPPER){
			contact->SetEnabled(false); //No collision between flippers
			return;
		}else{
			return;
		}

		if(otherObject_data->type == UserData::PINBALL_PINS){
			//TODO: otherObject->reward;
			return;
		}else if(otherObject_data->type == UserData::PINBALL_KICKER){
			ball->ApplyForceToCenter(b2Vec2(KICKER_FORCE_X, KICKER_FORCE_Y), true);
			return;
		}else if(otherObject_data->type == UserData::PINBALL_GAMEOVER){
			//TODO: big negative reward
			//TODO: move ball above the kicker
			gameOverCallback();
			return;
		}
	}
}
