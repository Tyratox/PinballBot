/*
 * State.cpp
 * Contains all the information of a state
 *
 */

#ifndef PINBALL_BOT_STATE
#define PINBALL_BOT_STATE

#include "Ball.cpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

class State{

	private:

		Ball ball;

		/**
		 * Generates a seed.
		 * @return	void
		 */
		static unsigned int seed(){
			return (unsigned int) rand();//FIXME not the best solution but works for now.
		}

		/**
		 * Generates a pseudo-random float
		 * @param	min		float		The minimum possible number to generate [included]
		 * @param	max		float		The maximum possible number to generate [included]
		 * @return			float		The pseudo random number
		 */
		static float randomFloatInRange(const float &min, const float &max){
			std::default_random_engine					generator(seed());

			std::uniform_real_distribution<float>		distribution
			= std::uniform_real_distribution<float>(min, max);

			return distribution(generator);
		}

		/**
		 * Generates a pseudo-random int
		 * @param	min		int			The minimum possible number to generate [included]
		 * @param	max		int			The maximum possible number to generate [included]
		 * @return			int			The pseudo random number
		 */
		static int randomIntInRange(const int &min, const int &max){
			std::default_random_engine					generator(seed());

			std::uniform_int_distribution<int>		distribution
				= std::uniform_int_distribution<int>(min, max);

			return distribution(generator);
		}

		/**
		 * Reduces the precision of a floating point number in order to reduce the data load
		 * @param	precision	int			The amount of decimal places to round to
		 * @param	number		float		The floating point number to round
		 * @return				float		The rounded number
		 */
		float roundFloat(const int &precision, const float &number){
			return round(number * (10 * precision)) / (10 * precision);
		}

		/**
		 * Reduces the precision of a b2Vec
		 * @param	precision	int			The amount of decimal places to round floats to
		 * @param	vec			b2Vec2		The vector whose precision should be reduced
		 * @return				b2Vec2		The rounded number
		 */
		b2Vec2 reduceVectorPrecision(const int &precision, const b2Vec2 &vec){
			return b2Vec2(this->roundFloat(precision, vec.x), this->roundFloat(precision, vec.y));
		}

		/**
		 * Reduces the precision of the state in order to reduce the data load
		 * @param	precision	int			The amount of decimal places to round floats to
		 * @return				void
		 */
		void reduceStatePrecision(const int &precision){
			ball.setPosition(reduceVectorPrecision(precision, ball.getPosition()));
			ball.setVelocity(reduceVectorPrecision(precision, ball.getVelocity()));
		}


	public:

		//inits a state
		State(Ball ball){
			this->ball = ball;
			reduceStatePrecision(2);

			std::cout << this->ball.getPosition().x << " " << this->ball.getPosition().y << std::endl;
		}

		/**
		 * Gets the expected reward based on a lookup table and the model of the environment
		 * @param	s	State	The State of which the reward is calculated
		 * @return		double	The expected reward
		 */

		float getReward(){



			return 0.0f;
		}

		/**
		 * Calculates the expected value the given state will in the next turn and in the future => convergence
		 * @param	s	State	The State of which the value is calculated
		 * @return		double	The expected value
		 */

		float getValue(){



			return 0.0f;
		}

		/**
		 * Returns one state inside of a vector based on a epsilon greedy algorithm
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @param	epsilon		float					Range: [0-1]: The percentage of time which this function should pick a random state
		 * @return				State					The picked state
		 */
		static State epsilonGreedy(std::vector<State> states, const float &epsilon){
			if(epsilon < randomFloatInRange(0.0f, 1.0f)){
				//pick a greedy state
				return greedy(states);
			}else{
				//pick a random state
				return random(states);
			}
		}

		/**
		 * Picks the state with the highest value
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @return				State					The picked state
		 */
		static State greedy(std::vector<State> states){
			int					maxValue = 0;
			float				tmpValue;
			std::vector<State>	maxStates;
			for(int i=1;i<states.size();i++){
				tmpValue		= states[i].getValue();

				if(tmpValue > maxValue){
					maxValue = tmpValue;

					maxStates.clear();
					maxStates.push_back(states[i]);
				}else if(tmpValue == maxValue){
					maxStates.push_back(states[i]);
				}
			}

			if(maxStates.size() == 1){
				return maxStates[0];
			}else{
				return random(maxStates);
			}
		}

		/**
		 * Picks a random state
		 * @param	states		std::vector<State>		A vector containing all the possible states
		 * @return				State					The picked state
		 */
		static State random(const std::vector<State> &states){
			return states[randomIntInRange(0, states.size()-1)];
		}

};

#endif /* PINBALL_BOT_STATE */
