#ifndef PLANE_INCLUDED_H
#define PLANE_INCLUDED_H

#include "../util/math3d.h"
#include "boundingSphere.h"

class Plane {
public:
	Plane(const Vector3f& normal, float distance) : m_normal(normal), m_distance(distance) {}

	Plane normalized() const;

	IntersectData intersectSphere(const BoundingSphere& other) const;

	inline const Vector3f& getNormal() const { return m_normal; }
	inline float getDistance() const { return m_distance; }

	static void test();
private:
	const Vector3f m_normal;
	const float m_distance;
};

#endif