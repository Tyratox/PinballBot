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

	public:
		ContactListener(){}

		/// Called when two fixtures begin to touch.
		void BeginContact(b2Contact* contact) {
			if(contact->IsTouching() && contact->IsEnabled()){
				UserData *userDataA = (UserData*) contact->GetFixtureA()->GetBody()->GetUserData();
				UserData *userDataB = (UserData*) contact->GetFixtureB()->GetBody()->GetUserData();

				UserData *ball;
				UserData *otherObject;

				if(userDataA->type == UserData::PINBALL_BALL){
					ball			= userDataA;
					otherObject		= userDataB;
				}else if(userDataB->type == UserData::PINBALL_BALL){
					ball			= userDataB;
					otherObject		= userDataA;
				}else{
					return;
				}

				if(otherObject->type == UserData::PINBALL_PINS){
					//otherObject->reward;
					return;
				}
			}
		}
};

#endif /* PINBALL_BOT_CONTACT_LISTENER */
