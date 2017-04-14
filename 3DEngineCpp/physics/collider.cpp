#include "collider.h"
#include "boundingSphere.h"
#include <iostream>
#include <cstdlib>

IntersectData Collider::intersect(const Collider& other) const {
	if (m_type == TYPE_SPHERE && other.getType() == TYPE_SPHERE) {
		BoundingSphere* self = (BoundingSphere*) this;
		return self->intersectBoundingSphere((BoundingSphere&) other);
	}

	std::cerr << "Error: Collision not implemented between specified " << "colliders." << std::endl;
	exit(1);

	return IntersectData(false, 0);
}