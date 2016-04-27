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

		/// This is called after a contact is updated. This allows you to inspect a
		/// contact before it goes to the solver. If you are careful, you can modify the
		/// contact manifold (e.g. disable contact).
		/// A copy of the old manifold is provided so that you can detect changes.
		/// Note: this is called only for awake bodies.
		/// Note: this is called even when the number of contact points is zero.
		/// Note: this is not called for sensors.
		/// Note: if you set the number of contact points to zero, you will not
		/// get an EndContact callback. However, you may get a BeginContact callback
		/// the next step.
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold){

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
					return;
				}
			}
		}
};

#endif /* PINBALL_BOT_CONTACT_LISTENER */
