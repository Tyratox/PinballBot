/*
 * ActionsSim.cpp
 *
 * Combines all simulation actions
 */

#ifndef ACTIONS_SIM
#define ACTIONS_SIM

#include <iostream>
#include <vector>

#include "../sim/Simulation.h"

#include "Action.h"
#include "ActionSimEnableLeftFlipper.h"
#include "ActionSimDisableLeftFlipper.h"
#include "ActionSimEnableRightFlipper.h"
#include "ActionSimDisableRightFlipper.h"

class ActionsSim{
	private:
	public:
		static std::vector<Action*> actionsAvailable(Simulation &sim){

			std::vector<Action*> vec(4);

			vec[0] = new ActionSimEnableLeftFlipper(sim);
			vec[1] = new ActionSimDisableLeftFlipper(sim);
			vec[2] = new ActionSimEnableRightFlipper(sim);
			vec[3] = new ActionSimDisableRightFlipper(sim);

			return vec;
		}
};

#endif /* ACTIONS_SIM */
