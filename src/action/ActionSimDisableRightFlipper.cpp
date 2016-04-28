/*
 * ActionSimDisableRightFlipper.cpp
 *
 * Sim action that disables the right flipper
 */

#include "ActionSimDisableRightFlipper.h"
#include "Action.h"
#include "../sim/Simulation.h"

ActionSimDisableRightFlipper::ActionSimDisableRightFlipper(Simulation &sim) : sim(sim){

}

void ActionSimDisableRightFlipper::run(){
	sim.disableRightFlipper();
}
