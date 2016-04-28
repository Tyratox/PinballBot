/*
 * Ball.cpp
 * Stores information about a ball
 *
 */

#include <Box2D/Box2D.h>

#include "Ball.h"

Ball::Ball(b2Vec2 position, b2Vec2 velocity) : position(position), velocity(velocity){

}

b2Vec2 Ball::getPosition(){
	return this->position;
}

void Ball::setPosition(b2Vec2 position){
	this->position = position;
}


b2Vec2 Ball::getVelocity(){
	return this->velocity;
}

void Ball::setVelocity(b2Vec2 velocity){
	this->velocity = velocity;
}
