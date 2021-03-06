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
	engine->getGraphicsEngine()->setMainCamera(m_camera);
}

void CameraComponent::setParent(Entity* parent)
{
	EntityComponent::setParent(parent);
	m_camera.setTransformer(getTransformer());
}