#include "transformer.h"

Transformer::Transformer(const Vector3f& pos, const Quaternion& rot, float scale) {
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	m_initializedOldStuff = false;
	m_parent = 0;
	
	m_parentMatrix = Matrix4f().initIdentity();
}

bool Transformer::hasChanged() {	
	if(m_parent != 0 && m_parent->hasChanged())
		return true;
	
	if(m_pos != m_oldPos)
		return true;

	if(m_rot != m_oldRot)
		return true;
	
	if(m_scale != m_scale)
		return true;
		
	return false;
}

void Transformer::update() {
	if(m_initializedOldStuff) {
		m_oldPos = m_pos;
		m_oldRot = m_rot;
		m_oldScale = m_scale;
	} else {
		m_oldPos = m_pos + Vector3f(1,1,1);
		m_oldRot = m_rot * 0.5f;
		m_oldScale = m_scale + 1;
		m_initializedOldStuff = true;
	}
}

void Transformer::rotate(const Vector3f& axis, float angle) {
	rotate(Quaternion(axis, angle));
}

void Transformer::rotate(const Quaternion& rotation) {
	m_rot = Quaternion((rotation * m_rot).normalized());
}

void Transformer::lookAt(const Vector3f& point, const Vector3f& up) {
	m_rot = getLookAtRotation(point, up);
}

Matrix4f Transformer::getTransformation() const {
	Matrix4f translationMatrix;
	Matrix4f scaleMatrix;

	translationMatrix.initTranslation(Vector3f(m_pos.getX(), m_pos.getY(), m_pos.getZ()));
	scaleMatrix.initScale(Vector3f(m_scale, m_scale, m_scale));

	Matrix4f result = translationMatrix * m_rot.toRotationMatrix() * scaleMatrix;

	return getParentMatrix() * result;
}

Matrix4f Transformer::getParentMatrix() const {
	if(m_parent != 0 && m_parent->hasChanged())
		m_parentMatrix = m_parent->getTransformation();
		
	return m_parentMatrix;
}
