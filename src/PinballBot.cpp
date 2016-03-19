//============================================================================
// Name        : PinballBot.cpp
// Author      : Nico Hauser, David Schmid
// Version     : 0.1
// Description : A reinforcement learning agent that learns to play pinball
//============================================================================

#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "Ball.cpp"
#include "State.cpp"

#include "Simulation.cpp"

int main() {

	PinballSimulation* sim = new PinballSimulation();
	for(int i=0;i<60;i++){
		sim->step();
		sim->debugPlayingBall();
	}

	return 0;
}
