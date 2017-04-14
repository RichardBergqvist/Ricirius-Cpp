#ifndef PHYSICS_OBJECT_INCLUDED_H
#define PHYSICS_OBJECT_INCLUDED_H

#include "../util/math3d.h"
#include "boundingSphere.h" 

class PhysicsObject {
public:
	PhysicsObject(const Vector3f& position, const Vector3f& velocity, float radius) : m_position(position), m_velocity(velocity), m_radius(radius), m_boundingSphere(position, radius) {}

	void integrate(float delta);

	inline void setVelocity(const Vector3f& velocity) { m_velocity = velocity; }
	
	inline const Vector3f& getPosition() const { return m_position; }
	inline const Vector3f& getVelocity() const { return m_velocity; }
	inline float getRadius() const { return m_radius; }

	inline const Collider& getBoundingSphere() const { m_boundingSphere = BoundingSphere(m_position, m_radius); return m_boundingSphere; }

	static void test();
private:
	Vector3f m_position;
	Vector3f m_velocity;
	float m_radius;
	mutable BoundingSphere m_boundingSphere;
};

#endif