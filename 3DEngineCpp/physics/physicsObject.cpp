#include "physicsObject.h"
#include <iostream>
#include <cassert>

void PhysicsObject::integrate(float delta) {
	m_position += m_velocity * delta;
}

void PhysicsObject::test() {
	PhysicsObject test(Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 2.0f, 3.0f));
	
	test.integrate(20.0f);
	
	Vector3f testPos = test.getPosition();
	Vector3f testVel = test.getVelocity();
	
	assert(testPos.getX() == 20.0f);
	assert(testPos.getY() == 41.0f);
	assert(testPos.getZ() == 60.0f);
	
	assert(testVel.getX() == 1.0f);
	assert(testVel.getY() == 2.0f);
	assert(testVel.getZ() == 3.0f);
}