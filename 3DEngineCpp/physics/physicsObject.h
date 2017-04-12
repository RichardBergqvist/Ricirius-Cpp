#ifndef PHYSICS_OBJECT_INCLUDED_H
#define PHYSICS_OBJECT_INCLUDED_H

#include "../util/math3d.h"

class PhysicsObject {
public:
	PhysicsObject(const Vector3f& position, const Vector3f& velocity) : m_position(position), m_velocity(velocity) {}

	void integrate(float delta);

	inline const Vector3f& getPosition() const { return m_position; }
	inline const Vector3f& getVelocity() const { return m_velocity; }

	static void test();
private:
	Vector3f m_position;
	Vector3f m_velocity;
};

#endif