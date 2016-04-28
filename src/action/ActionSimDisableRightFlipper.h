/*
 * ActionSimDisableRightFlipper.h
 *
 * Sim action that disables the right flipper
 */

#ifndef ACTION_ACTIONSIMDISABLERIGHTFLIPPER_H_
#define ACTION_ACTIONSIMDISABLERIGHTFLIPPER_H_

#include "Action.h"
#include "../sim/Simulation.h"

class ActionSimDisableRightFlipper : public Action{

	Simulation &sim;

	private:
	public:
		ActionSimDisableRightFlipper(Simulation &sim);

		void run();
};


#endif /* ACTION_ACTIONSIMDISABLERIGHTFLIPPER_H_ */
