/*
 * UserData.cpp
 *
 *  User data stored in Box2D bodys
 */

#include "UserData.h"

UserData::UserData(Type type, int reward, bool filled,
		int red, int green, int blue, int alpha):

	type(type), reward(reward), filled(filled), red(red), green(green), blue(blue), alpha(alpha){

}
