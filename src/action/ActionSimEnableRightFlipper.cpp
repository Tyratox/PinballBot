/*
 * ActionSimEnableRightFlipper.cpp
 *
 * Sim Action that enables the right flipper
 *
 */

#ifndef PINBALL_BOT_ACTION_SIM_ENABLE_RIGHT_FLIPPER
#define PINBALL_BOT_ACTION_SIM_ENABLE_RIGHT_FLIPPER

#include "Action.cpp"
#include "../Simulation.cpp"

class ActionSimEnableRightFlipper : public Action{

	PinballSimulation &sim;

	private:
	public:
	ActionSimEnableRightFlipper(PinballSimulation &sim) : sim(sim){

		}

		void run(){
			sim.enableRightFlipper();
		}
};

#endif /* PINBALL_BOT_ACTION_SIM_ENABLE_RIGHT_FLIPPER */
