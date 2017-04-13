#include "physicsObject.h"
#include <iostream>
#include <cassert>

void PhysicsObject::integrate(float delta) {
	m_position += m_velocity * delta;
}

void PhysicsObject::test() {
	PhysicsObject test(Vector3f(0, 1, 0), Vector3f(1, 2, 3), 1);
	
	test.integrate(20);
	
	Vector3f testPos = test.getPosition();
	Vector3f testVel = test.getVelocity();
	
	assert(testPos.getX() == 20);
	assert(testPos.getY() == 41);
	assert(testPos.getZ() == 60);
	
	assert(testVel.getX() == 1);
	assert(testVel.getY() == 2);
	assert(testVel.getZ() == 3);
}