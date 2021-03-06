/*
 * ListenerContactPoints.cpp
 *
 *  A contact listener for bodys
 */

#include <Box2D/Box2D.h>

#include <functional>
#include <random>
#include <chrono>
#include <stdio.h>

#include "ContactListener.h"
#include "UserData.h"
#include "../action/Action.h"
#include "Simulation.h"

const float ContactListener::KICKER_FORCE_Y_MIN		= 0.00075f;
const float ContactListener::KICKER_FORCE_Y_MAX		= 0.001f;

const bool	ContactListener::RANDOM_KICKER_FORCE	= false;

ContactListener::ContactListener(std::function<void(void)> gameOverCallback, std::function<void(float)> rewardCallback, bool randomKickerForce):
	gameOverCallback(gameOverCallback),
	rewardCallback(rewardCallback),
	generator(seed()),
	randomKickerForce(randomKickerForce)
	{};

unsigned ContactListener::seed(){
	return (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
}

float ContactListener::randomFloatInRange(const float &min, const float &max){
	std::uniform_real_distribution<float>		distribution
	= std::uniform_real_distribution<float>(min, max);

	return distribution(generator);
}

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

		if(otherObject_data->type == UserData::PINBALL_PIN){
			rewardCallback(otherObject_data->reward);
			return;
		}else if(otherObject_data->type == UserData::PINBALL_KICKER){

			if(RANDOM_KICKER_FORCE){
				ball->ApplyForceToCenter(b2Vec2(0.0f, randomFloatInRange(KICKER_FORCE_Y_MIN, KICKER_FORCE_Y_MAX)), true);
			}else{
				ball->ApplyForceToCenter(b2Vec2(0.0f, (KICKER_FORCE_Y_MIN + KICKER_FORCE_Y_MAX) / 2.0f), true);
			}


			return;
		}else if(otherObject_data->type == UserData::PINBALL_GAMEOVER){
			rewardCallback(Action::MIN_REWARD);
			gameOverCallback();
			return;
		}
	}
}
