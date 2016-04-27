/*
 * UserData.cpp
 *
 *  User data stored in Box2D bodys
 */

#ifndef PINBALL_BOT_SIM_USER_DATA
#define PINBALL_BOT_SIM_USER_DATA


class UserData{
	private:

	public:

		enum Type{
			PINBALL_BORDER,
			PINBALL_BALL,
			PINBALL_FLIPPER,
			PINBALL_KICKER,
			PINBALL_PINS,
			PINBALL_GAMEOVER
		};

		Type type;
		int reward; /* The reward given on collision, zero if none */

		UserData(Type type, int reward = 0) : type(type), reward(reward){

		}
};

#endif /* PINBALL_BOT_SIM_USER_DATA */
