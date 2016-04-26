/*
 * ActionSimDisableLeftFlipper.cpp
 *
 * Sim Action that disables the left flipper
 *
 */

#ifndef PINBALL_BOT_ACTION_SIM_DISABLE_LEFT_FLIPPER
#define PINBALL_BOT_ACTION_SIM_DISABLE_LEFT_FLIPPER

#include "Action.cpp"
#include "../sim/Simulation.cpp"

class ActionSimDisableLeftFlipper : public Action{

	PinballSimulation &sim;

	private:
	public:
		ActionSimDisableLeftFlipper(PinballSimulation &sim) : sim(sim){

		}

		void run(){
			sim.disableLeftFlipper();
		}
};

#endif /* PINBALL_BOT_ACTION_SIM_DISABLE_LEFT_FLIPPER */
