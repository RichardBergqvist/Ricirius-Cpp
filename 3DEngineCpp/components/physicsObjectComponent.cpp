#include "physicsObjectComponent.h"

void PhysicsObjectComponent::update(float delta) {
	getTransformer()->setPos(m_physicsObject->getPosition());
}