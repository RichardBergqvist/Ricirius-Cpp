#include "camera.h"
#include "../graphics/graphicsEngine.h"
#include "../core/coreEngine.h"

void Camera::addToEngine(CoreEngine* engine) const {
	engine->getGraphicsEngine()->addCamera(*this);
}

Matrix4f Camera::getViewProjection() const {
	Matrix4f cameraRotation = getTransformer().getTransformedRot().conjugate().toRotationMatrix();
	Matrix4f cameraTranslation;
	
	cameraTranslation.initTranslation(getTransformer().getTransformedPos() * -1);
	
	return m_projection * cameraRotation * cameraTranslation;
}
