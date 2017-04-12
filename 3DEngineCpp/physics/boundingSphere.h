#ifndef BOUNDING_SPHERE_INCLUDED_H
#define BOUNDING_SPHERE_INCLUDED_H

#include "../util/math3d.h"
#include "intersectData.h"

class BoundingSphere {
public:
	BoundingSphere(const Vector3f& center, float radius) : m_center(center), m_radius(radius) {}

	IntersectData intersectBoundingSphere(const BoundingSphere& other) const;

	inline const Vector3f& getCenter() const { return m_center; }
	inline float getRadius() const { return m_radius; }

	static void test();
private: 
	const Vector3f m_center;
	const float m_radius;
};

#endif