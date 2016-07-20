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
#include <string>
#include <numeric>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "PinballBot.h"

#include "action/ActionsSim.cpp"

#include "sim/Simulation.h"
#include "sim/Renderer.h"

#include "agent/Agent.h"
#include "agent/State.h"

#include "stats/StatsLogger.h"

const bool						PinballBot::SIMULATION					= true;

const bool						PinballBot::RENDER						= false;
const float						PinballBot::FPS							= 60.0f;
const float						PinballBot::TIME_STEP					= 1.0f / FPS;
const float						PinballBot::TICK_INTERVAL				= 1000.0f / FPS;

const unsigned long long		PinballBot::SAVE_INTERVAL				= 100000;
const unsigned long long		PinballBot::STATS_INTERVAL				= 50000;
const unsigned long long		PinballBot::LOG_INTERVAL				= 10000;
const unsigned long long		PinballBot::OUTSIDE_CF_UNTIL_RESPAWN	= 1800;//1 step ≈ 1/60 sec in-game, 1800 steps ≈ 30 secs in-game

const std::string				PinballBot::STATS_FILE					= "stats.csv";
const std::string				PinballBot::POLICIES_FILE				= "policies.csv";

PinballBot::PinballBot() : statsLogger(), rewardsCollected(0, 0.0f){

	KEYS							= SDL_GetKeyboardState(NULL);

	pause							= false;
	quit							= false;

	nextTime						= 0;

	steps							= 0;
	statsRewardsCollected			= 0;
	timeLastLog						= std::time(nullptr);
	gameOvers						= 0;

	stepStartedBeingOutsideCF		= 0;

	rlAgent							= nullptr;
	renderer						= nullptr;

	statsLogger.registerLoggingColumn("STEPS",					std::bind(&PinballBot::logSteps, this));
	statsLogger.registerLoggingColumn("TIME",					std::bind(&PinballBot::logTime, this));
	statsLogger.registerLoggingColumn("AMOUNT_OF_STATES",		std::bind(&PinballBot::logAmountOfStates, this));
	statsLogger.registerLoggingColumn("AVERAGE_TIME_PER_LOOP",	std::bind(&PinballBot::logAverageTimePerLoop, this));
	statsLogger.registerLoggingColumn("REWARDS_COLLECTED",		std::bind(&PinballBot::logRewardsCollected, this));
	statsLogger.registerLoggingColumn("GAMEOVERS",				std::bind(&PinballBot::logGameOvers, this));

	statsLogger.initLog(STATS_FILE);
}

Uint32 PinballBot::timeLeft() {
    Uint32 now = SDL_GetTicks();

    if(nextTime <= now){
    	return 0;
    }else{
    	return nextTime - now;
    }
}

void PinballBot::capFramerate() {
	SDL_Delay(timeLeft());
	nextTime += TICK_INTERVAL;
}

void PinballBot::handleKeys(Simulation &sim, SDL_Event &e){
	while( SDL_PollEvent( &e ) != 0 ){
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

bool PinballBot::preventStablePositionsOutsideCF(Simulation &sim){
	if(sim.isPlayingBallInsideCaptureFrame()){

		stepStartedBeingOutsideCF = 0;
		return true;
	}else{
		if(stepStartedBeingOutsideCF == 0){
			//when does the ball leave the CF
			stepStartedBeingOutsideCF		= steps;
		}else{
			//if it stays out of it for too long, respawn it
			if((steps - stepStartedBeingOutsideCF) > OUTSIDE_CF_UNTIL_RESPAWN){
				printf("The ball was outside the capture frame for too long, respawn!\n");
				sim.debugPlayingBall();
				sim.respawnBall();

				stepStartedBeingOutsideCF = 0;
			}
		}

		return false;
	}
}

void PinballBot::runSimulation(){

	Simulation 										sim;
	SDL_Event										e;

	std::vector<Action*> availableActions			= ActionsSim::actionsAvailable(sim);

	Agent											agent(availableActions);
	rlAgent											= &agent;

	if(RENDER){
		renderer			= new Renderer(320, 640, sim.getWorld());
	}

	while(!quit){

		if(RENDER){
			handleKeys(sim, e);
		}

		if(!pause){

			sim.step(TIME_STEP);

			//Ignore default rewards
			if(sim.reward != Action::DEFAULT_REWARD){
				rewardsCollected.push_back(sim.reward);
			}

			if(sim.reward == 0.0f){
				gameOvers++;
			}

			if(preventStablePositionsOutsideCF(sim)){
				rlAgent->think(sim.getCurrentState(availableActions), rewardsCollected);
				statsRewardsCollected += std::accumulate(rewardsCollected.begin(), rewardsCollected.end(), 0.0f);
				rewardsCollected.clear();
			}

			if(RENDER){
				renderer->render();
				capFramerate();
			}

		}else{
			nextTime = SDL_GetTicks() + TICK_INTERVAL;
		}

		if(steps != 0){
			if(steps % LOG_INTERVAL  == 0){
				printf("step #%lld | amount of states: %ld\n", steps, rlAgent->states.size());

				if(steps % STATS_INTERVAL == 0){
					statsLogger.log(STATS_FILE);

					statsRewardsCollected = 0;
					gameOvers = 0;

					if(steps % SAVE_INTERVAL == 0){
						rlAgent->savePoliciesToFile();
					}
				}

			}
		}
		steps++;
	}
}

void PinballBot::shutdownHook(){
	rlAgent->savePoliciesToFile(); //doesn't work yet, vector empty :/

	//archive previous log file
	statsLogger.archiveLog(STATS_FILE);
}


std::string PinballBot::logSteps(){
	return std::to_string(steps);
}

std::string PinballBot::logTime(){
	return std::to_string(std::time(nullptr));
}

std::string PinballBot::logAmountOfStates(){
	return std::to_string(rlAgent->states.size());
}

std::string PinballBot::logAverageTimePerLoop(){
	std::string r	= std::to_string(( ((double)(std::time(nullptr) - timeLastLog)) / ( (double)SAVE_INTERVAL) ));
	timeLastLog		= std::time(nullptr);

	return r;
}

std::string PinballBot::logRewardsCollected(){
	return std::to_string(statsRewardsCollected);
}

std::string PinballBot::logGameOvers(){
	return std::to_string(gameOvers);
}

void initLogFile(){
	std::ofstream statsFile;
	statsFile.open(PinballBot::STATS_FILE);
	statsFile << "STEPS;TIME;AMOUNT_OF_STATES;AVERAGE_TIME_PER_LOOP;REWARDS_COLLECTED;GAMEOVERS" << std::endl;
}

int main(int argc, char** argv) {

	PinballBot bot;

	//atexit(shutdownHook);

	if(PinballBot::SIMULATION){
		bot.runSimulation();
	}

	return 0;
}
