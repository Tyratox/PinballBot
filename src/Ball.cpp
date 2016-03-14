/*
 * Ball.cpp
 * Stores information about a ball
 *
 */

#ifndef PINBALL_BOT_BALL
#define PINBALL_BOT_BALL

#include <boost/numeric/ublas/vector.hpp>

class Ball{
	private:
		boost::numeric::ublas::vector<double> position;
		boost::numeric::ublas::vector<double> velocity;
		boost::numeric::ublas::vector<double> acceleration;

		/**
		 * Checks if a given vector is valid to use
		 * @param	v	boost::numeric::ublas::vector<double>	The vector to check
		 * @return		bool									True if the vector is valid,
		 * 														false if it isn't
		 */

		bool is_vector_valid(boost::numeric::ublas::vector<double> v){
			if(v.size() == 2 || v.size() == 3){ /* Only 2 and 3 dimensional vectors make sense */
				return true;
			}

			return false;
		}

	public:

		/**
		 * Constructor without any arguments, defaults to 3-dimensional vector
		 */
		Ball() : Ball(3) {};

		Ball(int dimensions){
			if(this->is_vector_valid(boost::numeric::ublas::vector<double>(dimensions))){
				this->set_position(boost::numeric::ublas::vector<double>(dimensions));
				this->set_velocity(boost::numeric::ublas::vector<double>(dimensions));
				this->set_acceleration(boost::numeric::ublas::vector<double>(dimensions));
			}
		}

		/**
		 * Updates the ball position
		 * @param	position	boost::numeric::ublas::vector<double>	The new ball position
		 * @return				bool									True if the variable was successfully updated,
		 * 																false if an error occurred
		 */
		bool set_position(boost::numeric::ublas::vector<double> position){

			if(this->is_vector_valid(position)){
				this->position = position;

				return true;
			}

			return false;
		}

		/**
		 * Retrieves the current ball position
		 * @return	boost::numeric::ublas::vector<double>	The current ball position
		 */
		boost::numeric::ublas::vector<double> get_position(){
			return this->position;
		}

		/**
		 * Updates the ball velocity
		 * @param	velocity	boost::numeric::ublas::vector<double>	The new ball velocity
		 * @return					bool									True if the variable was successfully updated,
		 * 																	false if an error occurred
		 */
		bool set_velocity(boost::numeric::ublas::vector<double> velocity){
			if(this->is_vector_valid(velocity)){
				this->velocity = velocity;

				return true;
			}

			return false;
		}

		/**
		 * Retrieves the current ball velocity
		 * @return	boost::numeric::ublas::vector<double>	The current ball velocity
		 */
		boost::numeric::ublas::vector<double> get_velocity(){
			return this->velocity;
		}

		/**
		 * Updates the ball position
		 * @param	acceleration	boost::numeric::ublas::vector<double>	The new ball acceleration
		 * @return						bool									True if the variable was successfully updated,
		 * 																		false if an error occurred
		 */
		bool set_acceleration(boost::numeric::ublas::vector<double> acceleration){
			if(this->is_vector_valid(acceleration)){
				this->acceleration = acceleration;

				return true;
			}

			return false;
		}

		/**
		 * Retrieves the current ball acceleration
		 * @return	boost::numeric::ublas::vector<double>	The current ball acceleration
		 */
		boost::numeric::ublas::vector<double> get_acceleration(){
			return this->acceleration;
		}
};

#endif /* PINBALL_BOT_BALL */
