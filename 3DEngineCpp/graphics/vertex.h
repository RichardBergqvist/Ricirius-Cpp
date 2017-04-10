#ifndef VERTEX_H
#define VERTEX_H

#include "../util/math3d.h"

class Vertex {
public:
	Vertex(Vector3f pos, Vector2f texCoord = Vector2f(0, 0), Vector3f normal = Vector3f(0, 0, 0), Vector3f tangent = Vector3f(0, 0, 0)) : m_pos(pos), m_texCoord(texCoord), m_normal(normal), m_tangent(tangent) {}

	inline void setPos(const Vector3f& pos) { m_pos = pos; }
	inline void setTexCoord(const Vector2f& texCoord) { m_texCoord = texCoord; }
	inline void setNormal(const Vector3f& normal) { m_normal = normal; }
	inline void setTangent(const Vector3f& tangent) { m_tangent = tangent; }

	inline const Vector3f& getPos() const { return m_pos; }
	inline const Vector2f& getTexCoord() const { return m_texCoord; }
	inline const Vector3f& getNormal() const { return m_normal; }
	inline const Vector3f& getTangent() const { return m_tangent; }
private:
	Vector3f m_pos;
	Vector2f m_texCoord;
	Vector3f m_normal;
	Vector3f m_tangent;
};

#endif