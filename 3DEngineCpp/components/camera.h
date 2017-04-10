#ifndef CAMERA_H
#define CAMERA_H

#include "../util/math3d.h"
#include "../components/gameComponent.h"

class Camera : public GameComponent {
public:
	Camera(const Matrix4f& projection) : m_projection(projection) {}
	
	virtual void addToEngine(CoreEngine* engine) const;

	inline void setProjection(const Matrix4f& projection) { m_projection = projection; }
	Matrix4f getViewProjection() const;
protected:
private:
	Matrix4f m_projection;
};

#endif
