#ifndef PHYSICS_ENGINE_COMPONENT_INCLUDED_H
#define PHYSICS_ENGINE_COMPONENT_INCLUDED_H

#include "entityComponent.h"
#include "../physics/physicsEngine.h"

class PhysicsEngineComponent : public EntityComponent {
public:
	PhysicsEngineComponent(const PhysicsEngine& engine) : m_physicsEngine(engine) {}

	virtual void update(float delta);

	inline const PhysicsEngine& getPhysicsEngine() { return m_physicsEngine; }
private:
	PhysicsEngine m_physicsEngine;
};

#endif