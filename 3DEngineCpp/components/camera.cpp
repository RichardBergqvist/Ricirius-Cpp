#include "../components/camera.h"
#include "../graphics/graphicsEngine.h"
#include "../core/coreEngine.h"

Camera::Camera(const Matrix4f& projection) :
	m_projection(projection) {}

Matrix4f Camera::getViewProjection() const {
	Matrix4f cameraRotation = getTransformer().getTransformedRot().conjugate().toRotationMatrix();
	Matrix4f cameraTranslation;
	
	cameraTranslation.initTranslation(getTransformer().getTransformedPos() * -1);
	
	return m_projection * cameraRotation * cameraTranslation;
}

void Camera::addToEngine(CoreEngine* engine) {
	engine->getGraphicsEngine()->addCamera(this);
}
