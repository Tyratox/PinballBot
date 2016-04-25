//============================================================================
// Name        : PinballBot.cpp
// Author      : Nico Hauser, David Schmid
// Version     : 0.1
// Description : A reinforcement learning agent that learns to play pinball
//============================================================================

#include <iostream>
#include <math.h>

#include <boost/numeric/ublas/vector.hpp>

#include <Box2D/Box2D.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "action/ActionsSim.cpp"

#include "Ball.cpp"
#include "State.cpp"

#include "Simulation.cpp"
#include "Renderer.cpp"

const bool					SIMULATION			= true;

const bool					RENDER				= true;
const float					FPS					= 60.0f;
const float					TIME_STEP			= 1.0f / FPS;
const float					TICK_INTERVAL		= 1000.0f / FPS;

const Uint8*				KEYS				= SDL_GetKeyboardState(NULL);

bool						quit				= false;

static Uint32				next_time			= 0;

Uint32 timeLeft(void) {
    Uint32 now = SDL_GetTicks();

    if(next_time <= now){
    	return 0;
    }else{
    	return next_time - now;
    }
}

void capFramerate(void) {
	SDL_Delay(timeLeft());
	next_time += TICK_INTERVAL;
}

void runSimulation(){

	PinballSimulation 		sim;
	Renderer				r(320, 640);
	SDL_Event				e;

	std::vector<Action> actions = ActionsSim::actionsAvailable(sim);

	if(RENDER){
		r.SetFlags( b2Draw::e_shapeBit );

		sim.setRenderer(&r);
	}

	while(!quit){

		if(RENDER){
			while( SDL_PollEvent( &e ) != 0 ){
				//User requests quit
				if( e.type == SDL_QUIT ){
					quit = true;
				}

				/* Just for debugging purposes */
				if (KEYS[SDL_SCANCODE_LEFT]){
					sim.enableLeftFlipper();
				}else{
					sim.disableLeftFlipper();
				}

				if (KEYS[SDL_SCANCODE_RIGHT]){
					sim.enableRightFlipper();
				}else{
					sim.disableRightFlipper();
				}

			}
		}

		sim.step(TIME_STEP);

		if(RENDER){
			sim.render();

			r.redraw();
			//sim.debugPlayingBall();
			sim.getCurrentState();
			capFramerate();
		}
	}
}

int main(int argc, char** argv) {

	if(SIMULATION){
		runSimulation();
	}


	return 0;
}
