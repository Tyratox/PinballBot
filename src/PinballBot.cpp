//============================================================================
// Name        : PinballBot.cpp
// Author      : Nico Hauser, David Schmid
// Version     : 0.1
// Description : A reinforcement learning agent that learns to play pinball
//============================================================================

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "action/ActionsSim.cpp"

#include "sim/Simulation.h"
#include "sim/Renderer.h"

const bool					SIMULATION			= true;

const bool					RENDER				= true;
const float					FPS					= 60.0f;
const float					TIME_STEP			= 1.0f / FPS;
const float					TICK_INTERVAL		= 1000.0f / FPS;

const Uint8*				KEYS				= SDL_GetKeyboardState(NULL);

bool						pause				= false;
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

	Simulation 				sim;
	Renderer				r(320, 640, sim.getWorld());
	SDL_Event				e;

	std::vector<Action> actions = ActionsSim::actionsAvailable(sim);

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

			}
		}

		if(!pause){

			sim.step(TIME_STEP);

				if(RENDER){
					r.render();
					//sim.debugPlayingBall();
					//sim.getCurrentState();
					capFramerate();
				}
		}else{
			next_time = SDL_GetTicks() + TICK_INTERVAL;
		}
	}
}

int main(int argc, char** argv) {

	if(SIMULATION){
		runSimulation();
	}


	return 0;
}
