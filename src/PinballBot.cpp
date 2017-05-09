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
#include <cmath>

#include <boost/program_options.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "PinballBot.h"

#include "action/ActionsSim.cpp"

#include "sim/Simulation.h"
#include "sim/Renderer.h"

#include "agent/Agent.h"
#include "agent/State.h"

#include "stats/StatsLogger.h"

const bool						PinballBot::DEFAULT_AGENT_ENABLED			= true;
const bool						PinballBot::DEFAULT_DYNAMIC_STEP_INCREMENT	= true;

const bool						PinballBot::DEFAULT_RENDER					= true;
const float						PinballBot::FPS								= 60.0f;
const float						PinballBot::TIME_STEP						= 1.0f / FPS;
const float						PinballBot::TICK_INTERVAL					= 1000.0f / FPS;

const bool						PinballBot::AGENT_INCLUDE_VELOCITY			= false;

//const unsigned long long		PinballBot::CLEAR_INTERVAL					= 10000000;
//const unsigned long long		PinballBot::SAVE_INTERVAL					= 216000;//≈1h in game time
const unsigned long long		PinballBot::LOG_INTERVAL					= 54000; //≈15 min in game time

const unsigned long long		PinballBot::DEFAULT_BASE_STATS_INTERVAL		= 5400;//≈15 min in game time
const unsigned int				PinballBot::DEFAULT_MAX_BASE_STATS_MULTIPLE	= 100;

const unsigned long long		PinballBot::OUTSIDE_CF_UNTIL_RESPAWN		= 1800;//1 step ≈ 1/60 sec in-game, 1800 steps ≈ 30 secs in-game

const unsigned long long		PinballBot::DEFAULT_QUIT_STEP				= 5184000;

const std::string				PinballBot::STATS_FILE						= "stats.csv";
const std::string				PinballBot::POLICIES_FILE					= "policies.csv";

PinballBot::PinballBot(
		bool agentEnabled, bool dynamicStepIncrement, bool render,
		unsigned long long baseStatsInterval, unsigned int maxBaseStatsMultiple
		) :
		statsLogger(), rewardsCollected(0, 0.0f),
		agentEnabled(agentEnabled), render(render), dynamicStepIncrement(dynamicStepIncrement),
		baseStatsInterval(baseStatsInterval), maxBaseStatsMultiple(maxBaseStatsMultiple){

	KEYS							= SDL_GetKeyboardState(NULL);

	pause							= false;
	quit							= false;

	nextTime						= 0;

	steps							= 0;
	statsRewardsCollected			= 0;
	timeLastLog						= std::time(nullptr);
	gameOvers						= 0;

	stepStartedBeingOutsideCF		= 0;
	nextStatsLog					= baseStatsInterval;
	deltaStatsLog					= baseStatsInterval;

	rlAgent							= nullptr;
	renderer						= nullptr;

	std::string per = " (per " + std::to_string(baseStatsInterval) + " )";

	statsLogger.registerLoggingColumn("STEPS",					std::bind(&PinballBot::logSteps, this));
	statsLogger.registerLoggingColumn("TIME",					std::bind(&PinballBot::logTime, this));
	statsLogger.registerLoggingColumn("AMOUNT_OF_STATES",		std::bind(&PinballBot::logAmountOfStates, this));
	statsLogger.registerLoggingColumn("EPSILON",				std::bind(&PinballBot::logEpsilon, this));
	statsLogger.registerLoggingColumn("REWARDS_COLLECTED"+per,	std::bind(&PinballBot::logRewardsCollected, this));
	statsLogger.registerLoggingColumn("GAMEOVERS"+per,			std::bind(&PinballBot::logGameOvers, this));
	statsLogger.registerLoggingColumn("SCORE"+per,				std::bind(&PinballBot::logScore, this));

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

	while( SDL_PollEvent( &e ) != 0){
		if( e.type == SDL_QUIT ){
			quit = true;
		}

		if(!agentEnabled){

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

		if (KEYS[SDL_SCANCODE_SPACE]){
			pause = true;
		}else{
			pause = false;
		}

		/*if (KEYS[SDL_SCANCODE_P]){
			sim.generateRandomPinField();
		}*/

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

void PinballBot::runSimulation(int statesToBackport, float valueAdjustFraction, float epsilon, unsigned long long quitStep, bool dynamicEpsilon, bool randomKickerForce){

	Simulation 										sim(randomKickerForce);
	SDL_Event										e;

	std::vector<Action*> availableActions			= ActionsSim::actionsAvailable(sim);

	Agent											agent(
			statesToBackport,
			valueAdjustFraction,
			epsilon,
			availableActions,
			quitStep,
			dynamicEpsilon
	);

	rlAgent											= &agent;

	if(render){
		renderer									= new Renderer(320, 640, sim.getWorld());
	}

	while(!quit){

		if(render){
			handleKeys(sim, e);
		}

		if(!pause){

			sim.step(TIME_STEP);

			//Ignore default rewards
			if(sim.reward != Action::DEFAULT_REWARD){
				rewardsCollected.push_back(sim.reward);
			}

			if(sim.reward == Action::MIN_REWARD){
				gameOvers++;
			}

			if(sim.reward == Action::MIN_REWARD || preventStablePositionsOutsideCF(sim)){
				if(agentEnabled){
					rlAgent->think(sim.getCurrentState(availableActions, AGENT_INCLUDE_VELOCITY), rewardsCollected, steps);
				}

				statsRewardsCollected += std::accumulate(rewardsCollected.begin(), rewardsCollected.end(), 0.0f);
				rewardsCollected.clear();
			}

			if(render){
				renderer->render(std::to_string((statsRewardsCollected - gameOvers)).c_str());
				capFramerate();
			}

			if(steps != 0){

				if(steps % LOG_INTERVAL == 0){
					printf("step #%lld | amount of states: %ld\n", steps, rlAgent->states.size());
				}

				if(steps >= nextStatsLog){
					statsLogger.log(STATS_FILE);

					statsRewardsCollected = 0;
					gameOvers = 0;

					/*Increase nextStatsLog with an quadratic function that reaches
					 * y = MAX_BASE_STATS_MULTIPLE at x = QUIT_STEP
					 * f(x) = (p-1)/q^2 * x^2 + 1
					 */
					if(dynamicStepIncrement && quitStep > 0){
						deltaStatsLog = (unsigned long long) std::round(baseStatsInterval *
								((((double)maxBaseStatsMultiple - 1.0f)/((double)quitStep * (double)quitStep)) * (steps * steps) + 1));

					}else{
						deltaStatsLog = baseStatsInterval;
					}

					nextStatsLog += deltaStatsLog;


					rlAgent->savePoliciesToFile();
				}

				/*if(steps % CLEAR_INTERVAL == 0){
					unsigned long previouseStateAmount = rlAgent->states.size();

					rlAgent->clearStates();
					sim.respawnBall();

					printf("Cleared %lu states, Reduced size from %lu to %lu\n",
							(previouseStateAmount - rlAgent->states.size()),
							previouseStateAmount,
							rlAgent->states.size()
					);
				}*/
			}

			steps++;

			if(quitStep != 0 && steps > quitStep){
				quit = true;
			}

		}else{
			nextTime = SDL_GetTicks() + TICK_INTERVAL;
		}

	}

}

void PinballBot::shutdownHook(){
	rlAgent->savePoliciesToFile(); //doesn't work yet, vector empty :/

	//archive previous log file
	statsLogger.archiveLog(STATS_FILE);
}

double PinballBot::normalizeReward(double reward){
	return (reward/(double)deltaStatsLog) * baseStatsInterval;
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
	std::string r	= std::to_string(( ((double)(std::time(nullptr) - timeLastLog)) / ( (double)deltaStatsLog) ));
	timeLastLog		= std::time(nullptr);

	return r;
}

std::string PinballBot::logEpsilon(){
	return std::to_string(rlAgent->getEpsilon(steps));
}

std::string PinballBot::logRewardsCollected(){
	return std::to_string(normalizeReward(statsRewardsCollected));
}

std::string PinballBot::logGameOvers(){
	return std::to_string(normalizeReward((double) gameOvers));
}

std::string PinballBot::logScore(){
	return std::to_string((normalizeReward(statsRewardsCollected) - normalizeReward((double) gameOvers)));
}

int main(int argc, char** argv) {
	//PinballBot

	bool agentEnabled;
	bool dynamicStepIncrement;
	bool render;
	unsigned long long baseStatsInterval;
	unsigned int maxBaseStatsMultiple;


	unsigned long long		quitStep;

	//Agent
	int						statesToBackport;
	float					valueAdjustFraction;
	float					epsilon;
	bool					dynamicEpsilon;

	//Sim
	bool					randomKickerForce;

	boost::program_options::options_description desc("Options for PinballBot");
	desc.add_options()

		//Produces a help message
		( "help,h", "produces a help message")

		// Option 'agent' and 'a' are equivalent.
		("agent,a", boost::program_options::value<bool>(& agentEnabled)->default_value(PinballBot::DEFAULT_AGENT_ENABLED),
			"Whether the agent should be enabled")
		// Option 'dynamic-step-increment' and 'd' are equivalent.
		("dynamic-step-increment,d", boost::program_options::value<bool>(& dynamicStepIncrement)->default_value(PinballBot::DEFAULT_DYNAMIC_STEP_INCREMENT),
			"Whether the agent should be enabled")
		// Option 'render' and 'r' are equivalent.
		("render,r", boost::program_options::value<bool>(& render)->default_value(PinballBot::DEFAULT_RENDER),
			"Whether to render the GUI")
		// Option 'base-stats-interval' and 'b' are equivalent.
		("base-stats-interval,b", boost::program_options::value<unsigned long long>(& baseStatsInterval)->default_value(PinballBot::DEFAULT_BASE_STATS_INTERVAL),
			"The base stats logging interval")
		// Option 'max-base-stats-multiple' and 'm' are equivalent.
		("max-base-stats-multiple,m", boost::program_options::value<unsigned int>(& maxBaseStatsMultiple)->default_value(PinballBot::DEFAULT_MAX_BASE_STATS_MULTIPLE),
			"The maximum multiple of the base stats logging interval")


		// Option 'states-to-backport' and 's' are equivalent.
		("states-to-backport,s", boost::program_options::value<int>(& statesToBackport)->default_value(Agent::DEFAULT_STATES_TO_BACKPORT),
			"The amount of states to backport")
		// Option 'value-adjust-fraction' and 'v' are equivalent.
		("value-adjust-fraction,v", boost::program_options::value<float>(& valueAdjustFraction)->default_value(Agent::DEFAULT_VALUE_ADJUST_FRACTION),
			"The fraction of the difference that should be added to the previous value")
		// Option 'epsilon' and 'e' are equivalent.
		("epsilon,e", boost::program_options::value<float>(& epsilon)->default_value(Agent::DEFAULT_EPSILON),
			"The epsilon for the epsilon greedy algo, 1.0 => random, 0.0 => 100% greedy")
		// Option 'quit-step' and 'q' are equivalent.
		("quit-step,q", boost::program_options::value<unsigned long long>(& quitStep)->default_value(PinballBot::DEFAULT_QUIT_STEP),
			"The amount of steps after which the program should quit")
		// Option 'dynamic-epsilon' and 'y' are equivalent.
		("dynamic-epsilon,y", boost::program_options::value<bool>(& dynamicEpsilon)->default_value(Agent::DEFAULT_DYNAMIC_EPSILON),
			"Whether to use a dynamic epsilon")

		// Option 'random-kicker-force' and 'f' are equivalent.
		("random-kicker-force,f", boost::program_options::value<bool>(& randomKickerForce)->default_value(ContactListener::RANDOM_KICKER_FORCE),
			"Whether to use a random kicker force")
	;

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);

	if (vm.count("help")){
		std::cout << "Usage: options_description [options]\n";
		std::cout << desc;
		return 0;
	}

	PinballBot bot(agentEnabled, dynamicStepIncrement, render, baseStatsInterval, maxBaseStatsMultiple);

	//atexit(shutdownHook);

	bot.runSimulation(statesToBackport, valueAdjustFraction, epsilon, quitStep, dynamicEpsilon, randomKickerForce);

	return 0;
}
