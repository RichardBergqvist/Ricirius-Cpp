#ifndef CAMERA_H
#define CAMERA_H

#include "math3d.h"
#include "gameComponent.h"

class Camera : public GameComponent {
public:
	Camera(const Matrix4f& projection);
	
	Matrix4f getViewProjection() const;
	virtual void addToEngine(CoreEngine* engine);
protected:
private:
	Matrix4f m_projection;
};

#endif
