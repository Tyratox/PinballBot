//============================================================================
// Name        : PinballBot.cpp
// Author      : Nico Hauser, David Schmid
// Version     : 0.1
// Description : A reinforcement learning agent that learns to play pinball
//============================================================================

#include <stdlib.h>     /* atexit */
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "action/ActionsSim.cpp"

#include "sim/Simulation.h"
#include "sim/Renderer.h"

#include "agent/Agent.h"
#include "agent/State.h"

static const bool					SIMULATION			= true;

static const bool					RENDER				= false;
static const float					FPS					= 60.0f;
static const float					TIME_STEP			= 1.0f / FPS;
static const float					TICK_INTERVAL		= 1000.0f / FPS;

static const unsigned long long		SAVE_INTERVAL		= 100000;
static const unsigned long long		DEBUG_INTERVAL		= 10000;
static const unsigned long long		SLEEP_UNTIL_RESPAWN	= 300;//1 step ≈ 1/60 sec in-game, 300 steps ≈ 5 secs in-game

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

	Simulation 									sim;
	SDL_Event									e;
	Agent										agent(ActionsSim::actionsAvailable(sim));
	rlAgent										= &agent;

	unsigned long long steps					= 0;
	double statsRewardsCollected				= 0;
	unsigned long long timeLastLog				= std::time(nullptr);
	unsigned long long gameOvers				= 0;

	unsigned long long stepStartedSleeping		= 0;

	if(RENDER){
		renderer			= new Renderer(320, 640, sim.getWorld());
	}



	std::vector<float> rewardsCollected(0, 0.0f);

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
			rewardsCollected.push_back(sim.reward);

			if(sim.reward == 0.0f){
				gameOvers++;
			}

			if(sim.isPlayingBallInsideCaptureFrame()){
				rlAgent->think(sim.getCurrentState(), rewardsCollected);
				statsRewardsCollected += std::accumulate(rewardsCollected.begin(), rewardsCollected.end(), 0.0f);
				rewardsCollected.clear();
			}

			if(sim.isPlayingBallSleeping()){
				/* The ball isn't moving anymore
				 * - The ball is stuck => not good, needs to respawn
				 */

				if(stepStartedSleeping == 0){
					//not slept until now
					stepStartedSleeping		= steps;
				}else{
					//was already sleeping, respawn if it is already stuck for a certain amount of steps
					if((steps - stepStartedSleeping) > SLEEP_UNTIL_RESPAWN){
						printf("Ball is stuck\n");
						sim.respawnBall();
					}
				}
			}else if(stepStartedSleeping != 0){
				stepStartedSleeping = 0;
			}

			if(RENDER){
				renderer->render();
				capFramerate();
			}

		}else{
			next_time = SDL_GetTicks() + TICK_INTERVAL;
		}

		if(steps != 0){
			if(steps % DEBUG_INTERVAL == 0){
				printf("step #%lld | amount of states: %ld\n", steps, rlAgent->states.size());

				if(steps % SAVE_INTERVAL == 0){
					rlAgent->savePoliciesToFile();

					//Log stats
					std::ofstream statsFile;

					statsFile.open("stats.csv", std::ios_base::app);//Append line
					//steps|current time|amount of states|∆time / ∆loops|rewards collected | gameovers
					statsFile <<
							steps << ";" <<
							std::time(nullptr) << ";" <<
							rlAgent->states.size() << ";" <<
							( ((double)(std::time(nullptr) - timeLastLog)) / ( (double)SAVE_INTERVAL) ) << ";" <<
							statsRewardsCollected << ";" <<
							gameOvers << std::endl;

					statsRewardsCollected = 0;
					gameOvers = 0;
				}

			}
		}
		steps++;
	}
}

void shutdownHook(){
	rlAgent->savePoliciesToFile(); //doesn't work yet, vector empty :/

	//archive previous log file
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer [20];

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	std::strftime(buffer, 20, "%Y-%m-%d-%H-%M", timeinfo);
	std::puts(buffer);

	rename("stats.csv", (std::string("stats-") + std::string(buffer) + std::string(".csv")).c_str());
}

void initLogFile(){
	std::ofstream statsFile;
	statsFile.open("stats.csv");
	statsFile << "STEPS;TIME;AMOUNT_OF_STATES;AVERAGE_TIME_PER_LOOP;REWARDS_COLLECTED;GAMEOVERS" << std::endl;
}

int main(int argc, char** argv) {
	atexit(shutdownHook);

	initLogFile();

	if(SIMULATION){
		runSimulation();
	}

	return 0;
}
