/*
 * ActionSimEnableRightFlipper.cpp
 *
 * Sim action that enables the right flipper
 */

#include "ActionSimEnableRightFlipper.h"
#include "Action.h"
#include "../sim/Simulation.h"

ActionSimEnableRightFlipper::ActionSimEnableRightFlipper(Simulation &sim) : sim(sim){

}

void ActionSimEnableRightFlipper::run(){
	sim.enableRightFlipper();
}
