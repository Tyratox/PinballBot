/*
 * Action.h
 *
 * An action that can be executed by the agent
 */

#ifndef ACTION_ACTION_H_
#define ACTION_ACTION_H_


class Action{
	private:

	public:

		Action();

		virtual ~Action();

		virtual void run();
};


#endif /* ACTION_ACTION_H_ */
