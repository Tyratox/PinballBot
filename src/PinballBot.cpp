//============================================================================
// Name        : PinballBot.cpp
// Author      : Nico Hauser, David Schmid
// Version     : 0.1
// Description : A reinforcement learning agent that learns to play pinball
//============================================================================

#include <stdlib.h>     /* atexit */
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "action/ActionsSim.cpp"

#include "sim/Simulation.h"
#include "sim/Renderer.h"

#include "agent/Agent.h"
#include "agent/State.h"

static const bool			SIMULATION			= true;

static const bool			RENDER				= true;
static const float			FPS					= 60.0f;
static const float			TIME_STEP			= 1.0f / FPS;
static const float			TICK_INTERVAL		= 1000.0f / FPS;

const Uint8*				KEYS				= SDL_GetKeyboardState(NULL);

bool						pause				= false;
bool						quit				= false;

static Uint32				next_time			= 0;

Agent*						rlAgent;

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

	Simulation 				sim;
	Renderer				r(320, 640, sim.getWorld());
	SDL_Event				e;

	std::vector<Action*>	availableActions = ActionsSim::actionsAvailable(sim);

	Agent					agent(availableActions);
	rlAgent					= &agent;

	//int step = 0;

	while(!quit){

		if(RENDER){
			while( SDL_PollEvent( &e ) != 0 ){
				//User requests quit
				if( e.type == SDL_QUIT ){
					quit = true;
				}

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

				if (KEYS[SDL_SCANCODE_SPACE]){
					pause = true;
				}else{
					pause = false;
				}

				if (KEYS[SDL_SCANCODE_P]){
					sim.generatePinField();
				}

				if (KEYS[SDL_SCANCODE_S]){
					rlAgent->savePolicyToFile();
				}

			}
		}

		if(!pause){

			sim.step(TIME_STEP);
			rlAgent->think(sim.getCurrentState(), sim.reward);

				if(RENDER){
					r.render();
					capFramerate();
				}
		}else{
			next_time = SDL_GetTicks() + TICK_INTERVAL;
		}

		/*if(step % 1000000 == 0){
			std::cout << "STEP: " << step << std::endl;
		}
		step++;*/
	}
}

void shutdownHook(){//doesn't work yet, vector empty :/
	rlAgent->savePolicyToFile();
}

int main(int argc, char** argv) {
	atexit(shutdownHook);

	if(SIMULATION){
		runSimulation();
	}


	return 0;
}
