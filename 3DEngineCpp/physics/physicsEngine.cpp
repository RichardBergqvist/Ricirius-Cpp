#include "PhysicsEngine.h"

void PhysicsEngine::addObject(const PhysicsObject& object) {
	m_objects.push_back(object);
}

void PhysicsEngine::simulate(float delta) {
	for (unsigned int i = 0; i < m_objects.size(); i++) {
		m_objects[i].integrate(delta);
	}
}