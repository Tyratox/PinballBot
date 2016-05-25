/*
 * ActionSimDisableLeftFlipper.cpp
 *
 * Sim action that disables the left flipper
 *
 */

#include "ActionSimDisableLeftFlipper.h"
#include "Action.h"
#include "../sim/Simulation.h"

ActionSimDisableLeftFlipper::ActionSimDisableLeftFlipper(Simulation &sim) : sim(sim){

}

void ActionSimDisableLeftFlipper::run(){
	sim.disableLeftFlipper();
}

const char* ActionSimDisableLeftFlipper::getUID(){return "DISABLE_LEFT_FLIPPER";}
