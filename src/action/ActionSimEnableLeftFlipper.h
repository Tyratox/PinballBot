/*
 * ActionSimEnableLeftFlipper.h
 *
 * Sim action that enables the left flipper
 */

#ifndef ACTION_ACTIONSIMENABLELEFTFLIPPER_H_
#define ACTION_ACTIONSIMENABLELEFTFLIPPER_H_


#include "Action.h"
#include "../sim/Simulation.h"

class ActionSimEnableLeftFlipper : public Action{

	Simulation &sim;

	private:
	public:
		ActionSimEnableLeftFlipper(Simulation &sim);

		void run();
		const char* getUID();
};

#endif /* ACTION_ACTIONSIMENABLELEFTFLIPPER_H_ */
