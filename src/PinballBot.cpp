//============================================================================
// Name        : PinballBot.cpp
// Author      : Nico Hauser, David Schmid
// Version     : 0.1
// Description : A reinforcement learning agent that learns to play pinball
//============================================================================

#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>
#include <unistd.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <Box2D/Box2D.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "Ball.cpp"
#include "State.cpp"

#include "Simulation.cpp"
#include "Renderer.cpp"

const bool					RENDER				= true;
const float					FPS					= 60.0f;
const float					TIME_STEP			= 1.0f / FPS;
const float					TICK_INTERVAL		= 1000.0f / FPS;

const Uint8*				KEYS				= SDL_GetKeyboardState(NULL);

bool						quit				= false;
bool						leftFlipper			= false;
bool						rightFlipper		= false;

Renderer					*r;
SDL_Event					e;

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

int main(int argc, char** argv) {

	PinballSimulation *sim = new PinballSimulation();

	if(RENDER){
		r = new Renderer(320, 640);
		r->SetFlags( b2Draw::e_shapeBit );

		sim->setRenderer(r);
	}

	while(!quit){

		if(RENDER){
			while( SDL_PollEvent( &e ) != 0 ){
				//User requests quit
				if( e.type == SDL_QUIT ){
					quit = true;
				}

				if (KEYS[SDL_SCANCODE_LEFT]){
					if(!leftFlipper){
						sim->enableLeftFlipper();
						leftFlipper = true;
					}
				}else if(leftFlipper){
					sim->disableLeftFlipper();
					leftFlipper = false;
				}

				if (KEYS[SDL_SCANCODE_RIGHT]){
					if(!rightFlipper){
						sim->enableRightFlipper();
						rightFlipper = true;
					}
				}else if(rightFlipper){
					sim->disableRightFlipper();
					rightFlipper = false;
				}

			}
		}

		sim->step(TIME_STEP);

		if(RENDER){
			sim->render();

			r->redraw();
			capFramerate();
		}
	}

	return 0;
}
