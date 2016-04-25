/*
 * Action.cpp
 * Interface/Virtual class for all actions available to the RL agent
 *
 */

#ifndef PINBALL_BOT_ACTION
#define PINBALL_BOT_ACTION

class Action{
	private:

	public:

		Action(){
		}

		virtual ~Action(){}

		virtual void run(){
			//The implementation of the action
		}
};

#endif /* PINBALL_BOT_ACTION */
