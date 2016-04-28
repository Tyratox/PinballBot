/*
 * UserData.h
 *
 *  User data stored in Box2D bodys
 */

#ifndef SIM_USERDATA_H_
#define SIM_USERDATA_H_

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

		UserData(Type type, int reward = 0);
};



#endif /* SIM_USERDATA_H_ */
