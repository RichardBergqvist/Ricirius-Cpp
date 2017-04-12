#include "aabb.h"
#include <cassert>

IntersectData AABB::intersectAABB(const AABB& other) const {
	Vector3f distances1 = other.getMinExtents() - m_maxExtents;
	Vector3f distances2 = m_minExtents - other.getMaxExtents();

	Vector3f distances = Vector3f(distances1.max(distances2));

	float maxDistance = distances.max();

	return IntersectData(maxDistance < 0, maxDistance);
}

void AABB::test() {
	AABB aabb1(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f));
	AABB aabb2(Vector3f(1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 2.0f));
	AABB aabb3(Vector3f(1.0f, 0.0f, 0.0f), Vector3f(2.0f, 1.0f, 1.0f));
	AABB aabb4(Vector3f(0.0f, 0.0f, -2.0f), Vector3f(1.0f, 1.0f, -1.0f));
	AABB aabb5(Vector3f(0.0f, 0.5f, 0.0f), Vector3f(1.0f, 1.5f, 1.0f));
	AABB aabb6(Vector3f(0.3f, 0.5f, 0.7f), Vector3f(1.3f, 1.5f, 1.7f));
	
	IntersectData aabb1Intersectaabb2 = aabb1.intersectAABB(aabb2);
	IntersectData aabb1Intersectaabb3 = aabb1.intersectAABB(aabb3);
	IntersectData aabb1Intersectaabb4 = aabb1.intersectAABB(aabb4);
	IntersectData aabb1Intersectaabb5 = aabb1.intersectAABB(aabb5);
	IntersectData aabb1Intersectaabb6 = aabb1.intersectAABB(aabb6);
	
	assert(aabb1Intersectaabb2.getDoesIntersect() == false);
	assert(aabb1Intersectaabb2.getDistance() == 0.0f);
	
	assert(aabb1Intersectaabb3.getDoesIntersect() == false);
	assert(aabb1Intersectaabb3.getDistance() == 0.0f);
	
	assert(aabb1Intersectaabb4.getDoesIntersect() == false);
	assert(aabb1Intersectaabb4.getDistance() == 1.0f);
	
	assert(aabb1Intersectaabb5.getDoesIntersect() == true);
	assert(aabb1Intersectaabb5.getDistance() == -0.5f);
	
	assert(aabb1Intersectaabb6.getDoesIntersect() == true);
	assert(aabb1Intersectaabb6.getDistance() == -0.3f);
}