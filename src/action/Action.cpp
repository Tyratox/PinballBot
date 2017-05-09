/*
 * Action.cpp
 *
 * An action that can be executed by the agent
 */

#include "Action.h"

const float Action::DEFAULT_REWARD	= 0.5f;
const float Action::MIN_REWARD		= 0.0f;
const float Action::MAX_REWARD		= 1.0f;

Action::Action(){}

Action::~Action(){}

void Action::run(){}

const char* Action::getUID(){return "NO_ACTION";}
