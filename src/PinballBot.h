/*
 * PinballBot.h
 *
 */

#ifndef PINBALLBOT_H_
#define PINBALLBOT_H_

#include <stdlib.h>     /* atexit */
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <ctime>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "action/ActionsSim.cpp"

#include "sim/Simulation.h"
#include "sim/Renderer.h"

#include "agent/Agent.h"
#include "agent/State.h"

#include "stats/StatsLogger.h"

class PinballBot{

	public:

		static const bool					AGENT;
		static const bool					DYNAMIC_STEP_INCREMENT;

		static const bool					RENDER;
		static const float					FPS;
		static const float					TIME_STEP;
		static const float					TICK_INTERVAL;

		static const float					AGENT_INCLUDE_VELOCITY;

		//static const unsigned long long	CLEAR_INTERVAL;
		//static const unsigned long long	SAVE_INTERVAL;
		static const unsigned long long		LOG_INTERVAL;

		static const unsigned long long		BASE_STATS_INTERVAL;
		static const unsigned int			MAX_BASE_STATS_MULTIPLE;

		static const unsigned long long		OUTSIDE_CF_UNTIL_RESPAWN;

		static const unsigned long long		DEFAULT_QUIT_STEP;

		static const std::string			STATS_FILE;
		static const std::string			POLICIES_FILE;

	private:

		const Uint8*						KEYS;

		bool								pause;
		bool								quit;

		Uint32								nextTime;

		Agent*								rlAgent;
		Renderer*							renderer;

		StatsLogger							statsLogger;

		unsigned long long 					steps;
		double 								statsRewardsCollected;
		unsigned long long 					timeLastLog;
		unsigned long long 					gameOvers;

		unsigned long long 					stepStartedBeingOutsideCF;
		unsigned long long					nextStatsLog;
		unsigned long long					deltaStatsLog;

		std::vector<float> 					rewardsCollected;

	public:

		PinballBot();

		/**
		 * Returns the time left for the next frame
		 * @return		Uint32
		 */
		Uint32 timeLeft();

		/**
		 * Delays the next frame if it was slower than TICK_INTERVAL
		 * @return		void
		 */
		void capFramerate();

		/**
		 * Handles the keys being pressed
		 * @param	sim	Simulation		The running simulation
		 * @param	e	SDL_Event		An sdl event
		 * @return		void
		 */

		void handleKeys(Simulation &sim, SDL_Event &e);

		/**
		 * Checks whether the ball is in- or outside the capture frame and if so for how long.
		 * If it stayed there longer than OUTSIDE_CF_UNTIL_RESPAWN, respawn the ball
		 * @return		bool	Whether the ball is inside the CF
		 */

		bool preventStablePositionsOutsideCF(Simulation &sim);

		/**
		 * Runs the simulation
		 * @param		argc	int		The amount of CLI arguments
		 * @param		argv	char**	The CLI arguments
		 * @return		void
		 */
		void runSimulation(int argc, char** argv);

		/**
		 * The main shutdown hook
		 * @return		void
		 */
		void shutdownHook();

		/**
		 * Calculates the reward for the last delta down to an comparable average
		 * @param	reward		double		The reward to normalize
		 * @returns double
		 */
		double normalizeReward(double reward);

		/**
		 * Logs the steps count
		 * @return		std::string
		 */

		std::string logSteps();

		/**
		 * Logs the current time
		 * @return		std::string
		 */

		std::string logTime();

		/**
		 * Logs the amount of states
		 * @return		std::string
		 */

		std::string logAmountOfStates();

		/**
		 * Logs the average time per loop
		 * @return		std::string
		 */

		std::string logAverageTimePerLoop();

		/**
		 * Logs the current agent epsilon
		 * @return		std::string
		 */

		std::string logEpsilon();

		/**
		 * Logs the rewards collected
		 * @return		std::string
		 */

		std::string logRewardsCollected();

		/**
		 * Logs the amount of gameovers
		 * @return		std::string
		 */

		std::string logGameOvers();

		/**
		 * Logs the score
		 * @return		std::string
		 */

		std::string logScore();

};

#endif /* PINBALLBOT_H_ */
