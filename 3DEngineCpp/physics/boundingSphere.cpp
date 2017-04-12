#include "boundingSphere.h"
#include <cassert>

IntersectData BoundingSphere::intersectBoundingSphere(const BoundingSphere& other) const {
	float radiusDistance = m_radius + other.getRadius();
	float centerDistance = (other.getCenter() - m_center).Length();
	float distance = centerDistance - radiusDistance;

	return IntersectData(distance < 0, distance);
}

void BoundingSphere::test() {
	BoundingSphere sphere1(Vector3f(0.0f, 0.0f, 0.0f), 1.0f);
	BoundingSphere sphere2(Vector3f(0.0f, 3.0f, 0.0f), 1.0f);
	BoundingSphere sphere3(Vector3f(0.0f, 0.0f, 2.0f), 1.0f);
	BoundingSphere sphere4(Vector3f(1.0f, 0.0f, 0.0f), 1.0f);
	
	IntersectData sphere1IntersectSphere2 = sphere1.intersectBoundingSphere(sphere2);
	IntersectData sphere1IntersectSphere3 = sphere1.intersectBoundingSphere(sphere3);
	IntersectData sphere1IntersectSphere4 = sphere1.intersectBoundingSphere(sphere4);
	
	assert(sphere1IntersectSphere2.getDoesIntersect() == false);
	assert(sphere1IntersectSphere2.getDistance() == 1.0f);
	
	assert(sphere1IntersectSphere3.getDoesIntersect() == false);
	assert(sphere1IntersectSphere3.getDistance() == 0.0f);
	
	assert(sphere1IntersectSphere4.getDoesIntersect() == true);
	assert(sphere1IntersectSphere4.getDistance() == -1.0f);
}