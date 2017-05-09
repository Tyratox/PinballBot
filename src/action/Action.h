/*
 * Action.h
 *
 * An action that can be executed by the agent
 */

#ifndef ACTION_ACTION_H_
#define ACTION_ACTION_H_


class Action{

	public:

		static const float	DEFAULT_REWARD;
		static const float	MIN_REWARD;
		static const float	MAX_REWARD;

	private:

	public:

		Action();

		virtual ~Action();

		virtual void run();

		virtual const char* getUID();
};


#endif /* ACTION_ACTION_H_ */
