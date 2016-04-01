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
const unsigned long long	TIME_STEP_MICROS	= round(TIME_STEP) * 10E5; /*E = 10^5 => 10*10^5 = 1'^6*/

bool						quit				= false;

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
		r = new Renderer(640, 480);
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
