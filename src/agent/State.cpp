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

		bool isLeftFlipperActive;
		bool isRightFlipperActive;

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
		State(Ball ball, bool isLeftFlipperActive, bool isRightFlipperActive) : isLeftFlipperActive(isLeftFlipperActive), isRightFlipperActive(isRightFlipperActive){
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

};

#endif /* PINBALL_BOT_STATE */
