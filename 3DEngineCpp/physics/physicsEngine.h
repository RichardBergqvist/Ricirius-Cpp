#ifndef PHYSICS_ENGINE_INCLUDED_H
#define PHYSICS_ENGINE_INCLUDED_H

#include "physicsObject.h"
#include <vector>

class PhysicsEngine {
public:
	PhysicsEngine() {}

	void addObject(const PhysicsObject& object);
	void simulate(float delta);

	void handleCollisions();

	inline const PhysicsObject& getObject(unsigned int index) const { return m_objects[index]; }
	inline unsigned int getNumObjects() const { return (unsigned int) m_objects.size(); }
private:
	std::vector<PhysicsObject> m_objects;

};

#endif