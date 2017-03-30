#ifndef CAMERA_H
#define CAMERA_H

#include "../util/math3d.h"
#include "../components/gameComponent.h"

class Camera : public GameComponent {
public:
	Camera(const Matrix4f& projection);
	
	Matrix4f getViewProjection() const;
	virtual void addToEngine(CoreEngine* engine);

	inline void setProjection(const Matrix4f& projection) { m_projection = projection; }
protected:
private:
	Matrix4f m_projection;
};

#endif
