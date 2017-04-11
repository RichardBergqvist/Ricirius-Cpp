#include "camera.h"
#include "../graphics/graphicsEngine.h"
#include "../core/coreEngine.h"

Matrix4f Camera::getViewProjection() const
{
	Matrix4f cameraRotation = getTransformer().getTransformedRot().conjugate().toRotationMatrix();
	Matrix4f cameraTranslation;

	cameraTranslation.initTranslation(getTransformer().getTransformedPos() * -1);

	return m_projection * cameraRotation * cameraTranslation;
}

void CameraComponent::addToEngine(CoreEngine* engine) const
{
	engine->setCamera(m_camera);
}

void CameraComponent::setParent(GameObject* parent)
{
	GameComponent::setParent(parent);
	m_camera.setTransformer(getTransformer());
}