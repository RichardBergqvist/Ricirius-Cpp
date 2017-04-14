#ifndef COLLIDER_INCLUDED_H
#define COLLIDER_INCLUDED_H

#include "intersectData.h"

class Collider {
public:
	enum {
		TYPE_SPHERE,
		TYPE_AABB,
		TYPE_SIZE
	};

	Collider(int type) : m_type(type) {}

	IntersectData intersect(const Collider& other) const;

	inline int getType() const { return m_type; }
private:
	int m_type;
};

#endif