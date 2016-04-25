/*
 * ActionSimEnableLeftFlipper.cpp
 *
 * Sim Action that enables the left flipper
 *
 */

#ifndef PINBALL_BOT_ACTION_SIM_ENABLE_LEFT_FLIPPER
#define PINBALL_BOT_ACTION_SIM_ENABLE_LEFT_FLIPPER

#include <iostream>

#include "Action.cpp"
#include "../Simulation.cpp"

class ActionSimEnableLeftFlipper : public Action{

	PinballSimulation &sim;

	private:
	public:
		ActionSimEnableLeftFlipper(PinballSimulation &sim) : sim(sim){
		}

		~ActionSimEnableLeftFlipper(){
		}

		void run(){
			sim.enableLeftFlipper();
		}
};

#endif /* PINBALL_BOT_ACTION_SIM_ENABLE_LEFT_FLIPPER */
