/*
 * ListenerContactPoints.cpp
 *
 *  A contact listener for bodys
 */

#include <iostream>

#include <Box2D/Box2D.h>

#include "UserData.cpp"

#ifndef PINBALL_BOT_CONTACT_LISTENER
#define PINBALL_BOT_CONTACT_LISTENER

class ContactListener: public b2ContactListener{
	private:

		const float KICKER_FORCE_X	= 0.0f;
		const float KICKER_FORCE_Y	= 1.0f;

	public:
		ContactListener(){}

		/// Called when two fixtures begin to touch.
		void BeginContact(b2Contact* contact) {
			if(contact->IsTouching() && contact->IsEnabled()){
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
					return;
				}
			}
		}
};

#endif /* PINBALL_BOT_CONTACT_LISTENER */
