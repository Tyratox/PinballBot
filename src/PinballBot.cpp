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

static const bool					SIMULATION			= true;

static const bool					RENDER				= true;
static const float					FPS					= 60.0f;
static const float					TIME_STEP			= 1.0f / FPS;
static const float					TICK_INTERVAL		= 1000.0f / FPS;

static const unsigned long long		SAVE_INTERVAL		= 100000;
static const unsigned long long		DEBUG_INTERVAL		= 10000;

const Uint8*						KEYS				= SDL_GetKeyboardState(NULL);

bool								pause				= false;
bool								quit				= false;

static Uint32						next_time			= 0;

Agent*								rlAgent;
Renderer*							renderer;

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

	if(RENDER){
		renderer			= new Renderer(320, 640, sim.getWorld());
	}

	SDL_Event				e;

	std::vector<Action*>	availableActions = ActionsSim::actionsAvailable(sim);

	Agent					agent(availableActions);
	rlAgent					= &agent;

	unsigned long long step = 0;

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
					sim.generateRandomPinField();
				}

				if (KEYS[SDL_SCANCODE_S]){
					rlAgent->savePoliciesToFile();
				}

			}
		}

		if(!pause){

			sim.step(TIME_STEP);
			rlAgent->think(sim.getCurrentState(), sim.reward);

				if(RENDER){
					renderer->render();
					capFramerate();
				}
		}else{
			next_time = SDL_GetTicks() + TICK_INTERVAL;
		}

		if(true){
			if(step % DEBUG_INTERVAL == 0){
				printf("STEP #%lld, State size: %ld\n", step, rlAgent->states.size());

				if(step % SAVE_INTERVAL == 0){
					rlAgent->savePoliciesToFile();
				}
			}
		}
		step++;
	}
}

void shutdownHook(){//doesn't work yet, vector empty :/
	rlAgent->savePoliciesToFile();
}

int main(int argc, char** argv) {
	atexit(shutdownHook);

	if(SIMULATION){
		runSimulation();
	}


	return 0;
}
