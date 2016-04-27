/*
 * ActionSimDisableRightFlipper.cpp
 *
 * Sim Action that disables the right flipper
 *
 */

#ifndef PINBALL_BOT_ACTION_SIM_DISABLE_RIGHT_FLIPPER
#define PINBALL_BOT_ACTION_SIM_DISABLE_RIGHT_FLIPPER

#include "Action.cpp"
#include "../sim/Simulation.cpp"

class ActionSimDisableRightFlipper : public Action{

	Simulation &sim;

	private:
	public:
		ActionSimDisableRightFlipper(Simulation &sim) : sim(sim){

		}

		void run(){
			sim.disableRightFlipper();
		}
};

#endif /* PINBALL_BOT_ACTION_SIM_DISABLE_RIGHT_FLIPPER */
