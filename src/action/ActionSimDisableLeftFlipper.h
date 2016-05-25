/*
 * ActionSimDisableLeftFlipper.h
 *
 * Sim action that disables the left flipper
 */

#ifndef ACTION_ACTIONSIMDISABLELEFTFLIPPER_H_
#define ACTION_ACTIONSIMDISABLELEFTFLIPPER_H_


#include "Action.h"
#include "../sim/Simulation.h"

class ActionSimDisableLeftFlipper : public Action{

	Simulation &sim;

	private:
	public:
		ActionSimDisableLeftFlipper(Simulation &sim);

		void run();
		const char* getUID();
};


#endif /* ACTION_ACTIONSIMDISABLELEFTFLIPPER_H_ */
