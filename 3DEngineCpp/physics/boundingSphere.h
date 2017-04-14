#ifndef BOUNDING_SPHERE_INCLUDED_H
#define BOUNDING_SPHERE_INCLUDED_H

#include "../util/math3d.h"
#include "intersectData.h"
#include "collider.h"

class BoundingSphere : public Collider {
public:
	BoundingSphere(const Vector3f& center, float radius) : Collider(Collider::TYPE_SPHERE), m_center(center), m_radius(radius) {}

	IntersectData intersectBoundingSphere(const BoundingSphere& other) const;

	inline const Vector3f& getCenter() const { return m_center; }
	inline float getRadius() const { return m_radius; }

	static void test();
private: 
	Vector3f m_center;
	float m_radius;
};

#endif