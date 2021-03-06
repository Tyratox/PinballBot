/*
 * ContactListener.h
 *
 *  A contact listener for bodys
 */

#ifndef SIM_CONTACTLISTENER_H_
#define SIM_CONTACTLISTENER_H_

#include <functional>
#include <random>

#include <Box2D/Box2D.h>

class ContactListener: public b2ContactListener{
	private:

		static const float					KICKER_FORCE_Y_MIN;
		static const float					KICKER_FORCE_Y_MAX;

		std::function<void(void)>			gameOverCallback;
		std::function<void(float)>			rewardCallback;

		std::default_random_engine			generator;

		unsigned seed();
		float randomFloatInRange(const float &min, const float &max);

	public:

		static const bool					RANDOM_KICKER_FORCE;
		const bool							randomKickerForce;

		ContactListener(std::function<void(void)> gameOverCallback, std::function<void(float)> rewardCallback, bool randomKickerForce);

		/// This is called after a contact is updated. This allows you to inspect a
		/// contact before it goes to the solver. If you are careful, you can modify the
		/// contact manifold (e.g. disable contact).
		/// A copy of the old manifold is provided so that you can detect changes.
		/// Note: this is called only for awake bodies.
		/// Note: this is called even when the number of contact points is zero.
		/// Note: this is not called for sensors.
		/// Note: if you set the number of contact points to zero, you will not
		/// get an EndContact callback. However, you may get a BeginContact callback
		/// the next step.
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif /* SIM_CONTACTLISTENER_H_ */
