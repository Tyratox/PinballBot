/*
 * ActionSimEnableLeftFlipper.cpp
 *
 * Sim action that enables the left flipper
 */

#include "ActionSimEnableLeftFlipper.h"
#include "Action.h"
#include "../sim/Simulation.h"

ActionSimEnableLeftFlipper::ActionSimEnableLeftFlipper(Simulation &sim) : sim(sim){

}

void ActionSimEnableLeftFlipper::run(){
	sim.enableLeftFlipper();
}

const char* ActionSimEnableLeftFlipper::getUID(){return "ENABLE_LEFT_FLIPPER";}
