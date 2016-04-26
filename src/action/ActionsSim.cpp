/*
 * ActionsSim.cpp
 *
 * Combines all simulation actions
 */

#include <iostream>
#include <vector>

#include "../sim/Simulation.cpp"

#include "Action.cpp"
#include "ActionSimEnableLeftFlipper.cpp"
#include "ActionSimDisableLeftFlipper.cpp"
#include "ActionSimEnableRightFlipper.cpp"
#include "ActionSimDisableRightFlipper.cpp"

class ActionsSim{
	private:
	public:
		static std::vector<Action> actionsAvailable(PinballSimulation &sim){

			std::vector<Action> vec(4);

			vec[0] = ActionSimEnableLeftFlipper(sim);
			vec[1] = ActionSimDisableLeftFlipper(sim);
			vec[2] = ActionSimEnableRightFlipper(sim);
			vec[3] = ActionSimDisableRightFlipper(sim);

			return vec;
		}
};
