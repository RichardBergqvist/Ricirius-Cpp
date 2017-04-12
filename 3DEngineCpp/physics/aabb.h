#ifndef AABB_INCLUDED_H
#define AABB_INCLUDED_H

#include "../util/math3d.h"
#include "intersectData.h"

class AABB {
public:
	AABB(const Vector3f& minExtents, const Vector3f& maxExtents) : m_minExtents(minExtents), m_maxExtents(maxExtents) {}

	IntersectData intersectAABB(const AABB& other) const;

	inline const Vector3f& getMinExtents() const { return m_minExtents; }
	inline const Vector3f& getMaxExtents() const { return m_maxExtents; }
	
	static void test();
private:
	const Vector3f m_minExtents;
	const Vector3f m_maxExtents;
};

#endif