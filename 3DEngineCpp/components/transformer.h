#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../util/math3d.h"

class Transformer {
public:
	Transformer(const Vector3f& pos = Vector3f(0,0,0), const Quaternion& rot = Quaternion(0,0,0,1), float scale = 1) : m_pos(pos), m_rot(rot), m_scale(scale), m_parent(0), m_parentMatrix(Matrix4f().initIdentity()), m_initializedOldStuff(false) {}

	Matrix4f getTransformation() const;
	bool hasChanged();
	void update();
	void rotate(const Vector3f& axis, float angle);
	void rotate(const Quaternion& rotation);
	void lookAt(const Vector3f& point, const Vector3f& up);
	
	Quaternion getLookAtRotation(const Vector3f& point, const Vector3f& up)  { 
		return Quaternion(Matrix4f().initRotationFromDirection((point - m_pos).normalized(), up)); 
	}

	inline void setPos(const Vector3f& pos) { m_pos = pos; }
	inline void setRot(const Quaternion& rot) { m_rot = rot; }
	inline void setScale(float scale) { m_scale = scale; }
	inline void setParent(Transformer* parent) { m_parent = parent; }

	inline Vector3f* getPos() { return &m_pos; }
	inline const Vector3f& getPos() const { return m_pos; }
	inline Quaternion* getRot() { return &m_rot; }
	inline const Quaternion& getRot() const { return m_rot; }
	inline float getScale() const { return m_scale; }
	inline Vector3f getTransformedPos()   const { return Vector3f(getParentMatrix().transform(m_pos)); }
	Quaternion getTransformedRot() const;

protected:
private:
	const Matrix4f& getParentMatrix() const;

	Vector3f m_pos;
	Quaternion m_rot;
	float m_scale;

	Transformer* m_parent;
	mutable Matrix4f m_parentMatrix;

	mutable Vector3f m_oldPos;
	mutable Quaternion m_oldRot;
	mutable float m_oldScale;
	mutable bool m_initializedOldStuff;
};

#endif
