#include "plane.h"
#include <cassert>

Plane Plane::normalized() const {
	float magnitude = m_normal.Length();

	return Plane(m_normal / magnitude, m_distance / magnitude);
}

IntersectData Plane::intersectSphere(const BoundingSphere& other) const {
	float distanceFromSphereCenter = (float) fabs(m_normal.dot(other.getCenter()) + m_distance);
	float distanceFromSphere = distanceFromSphereCenter - other.getRadius();

	return IntersectData(distanceFromSphere < 0, distanceFromSphere);
}

void Plane::test() {
	BoundingSphere sphere1(Vector3f(0.0f, 0.0f, 0.0f), 1.0f);
	BoundingSphere sphere2(Vector3f(0.0f, 3.0f, 0.0f), 1.0f);
	BoundingSphere sphere3(Vector3f(0.0f, 0.0f, 2.0f), 1.0f);
	BoundingSphere sphere4(Vector3f(1.0f, 0.0f, 0.0f), 1.0f);

	Plane plane1(Vector3f(0.0f, 1.0f, 0.0f), 0.0f);

	IntersectData plane1IntersectSphere1 = plane1.intersectSphere(sphere1);
	IntersectData plane1IntersectSphere2 = plane1.intersectSphere(sphere2);
	IntersectData plane1IntersectSphere3 = plane1.intersectSphere(sphere3);
	IntersectData plane1IntersectSphere4 = plane1.intersectSphere(sphere4);

	assert(plane1IntersectSphere1.getDoesIntersect() == true);
	assert(plane1IntersectSphere1.getDistance() == -1.0f);

	assert(plane1IntersectSphere2.getDoesIntersect() == false);
	assert(plane1IntersectSphere2.getDistance() == 2.0f);

	assert(plane1IntersectSphere3.getDoesIntersect() == true);
	assert(plane1IntersectSphere3.getDistance() == -1.0f);

	assert(plane1IntersectSphere4.getDoesIntersect() == true);
	assert(plane1IntersectSphere4.getDistance() == -1.0f);
}