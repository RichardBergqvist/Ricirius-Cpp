#ifndef CAMERA_H
#define CAMERA_H

#include "../util/math3d.h"
#include "entityComponent.h"

class Camera {
public:
	Camera(const Matrix4f& projection, Transformer* transformer) : m_projection(projection), m_transformer(transformer) {}

	inline void setProjection(const Matrix4f& projection) { m_projection = projection; }
	inline void setTransformer(Transformer* transformer) { m_transformer = transformer; }

	inline Transformer* getTransformer() { return m_transformer; }
	inline const Transformer& getTransformer() const { return *m_transformer; }
	Matrix4f getViewProjection() const;
protected:
private:
	Matrix4f   m_projection;
	Transformer* m_transformer;
};

class CameraComponent : public EntityComponent
{
public:
	CameraComponent(const Matrix4f& projection) : m_camera(projection, 0) {}

	virtual void addToEngine(CoreEngine* engine) const;

	inline Matrix4f getViewProjection() const { return m_camera.getViewProjection(); }

	inline void setProjection(const Matrix4f& projection) { m_camera.setProjection(projection); }
	virtual void setParent(Entity* parent);
protected:
private:
	Camera m_camera;
};

#endif