#ifndef PHYSICS_OBJECT_COMPONENT_INCLUDED_H
#define PHYSICS_OBJECT_COMPONENT_INCLUDED_H

#include "entityComponent.h"
#include "../physics/physicsEngine.h"

class PhysicsObjectComponent : public EntityComponent {
public:
	PhysicsObjectComponent(const PhysicsObject* object) : m_physicsObject(object) {}

	virtual void update(float delta);
private:
	const PhysicsObject* m_physicsObject;
};

#endif