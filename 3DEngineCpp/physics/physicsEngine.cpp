#include "PhysicsEngine.h"
#include "boundingSphere.h"

void PhysicsEngine::addObject(const PhysicsObject& object) {
	m_objects.push_back(object);
}

void PhysicsEngine::simulate(float delta) {
	for (unsigned int i = 0; i < m_objects.size(); i++) {
		m_objects[i].integrate(delta);
	}
}

void PhysicsEngine::handleCollisions() {
	for (unsigned int i = 0; i < m_objects.size(); i++) {
		for (unsigned int j = i + 1; j < m_objects.size(); j++) {
			IntersectData intersectData = m_objects[i].getBoundingSphere().intersectBoundingSphere(m_objects[j].getBoundingSphere());

			if (intersectData.getDoesIntersect()) {
				m_objects[i].setVelocity(m_objects[i].getVelocity() * -1);
				m_objects[j].setVelocity(m_objects[j].getVelocity() * -1);
			}
		}
	}
}