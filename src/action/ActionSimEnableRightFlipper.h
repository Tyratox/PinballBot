/*
 * ActionSimEnableRightFlipper.h
 *
 * Sim action that enables the right flipper
 */

#ifndef ACTION_ACTIONSIMENABLERIGHTFLIPPER_H_
#define ACTION_ACTIONSIMENABLERIGHTFLIPPER_H_


#include "Action.h"
#include "../sim/Simulation.h"

class ActionSimEnableRightFlipper : public Action{

	Simulation &sim;

	private:
	public:
		ActionSimEnableRightFlipper(Simulation &sim);

		void run();
};


#endif /* ACTION_ACTIONSIMENABLERIGHTFLIPPER_H_ */
