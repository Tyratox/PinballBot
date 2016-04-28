/*
 * ActionsSim.cpp
 *
 * Combines all simulation actions
 */

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
		static std::vector<Action> actionsAvailable(Simulation &sim){

			std::vector<Action> vec(4);

			vec[0] = ActionSimEnableLeftFlipper(sim);
			vec[1] = ActionSimDisableLeftFlipper(sim);
			vec[2] = ActionSimEnableRightFlipper(sim);
			vec[3] = ActionSimDisableRightFlipper(sim);

			return vec;
		}
};
