#include "../graphics/lighting.h"
#include "../graphics/graphicsEngine.h"
#include "../core/coreEngine.h"

#define COLOR_DEPTH 256

void BaseLight::addToEngine(CoreEngine* engine) const {
	engine->getGraphicsEngine()->addLight(*this);
}

ShadowCameraTransformer BaseLight::calcShadowCameraTransformer(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot) const {
	return ShadowCameraTransformer(getTransformer().getTransformedPos(), getTransformer().getTransformedRot());
}

DirectionalLight::DirectionalLight(const Vector3f& color, float intensity, int shadowMapSizeAsPowerOf2, float shadowArea, float shadowSoftness, float lightBleedReductionAmount, float minVariance) : BaseLight(color, intensity, Shader("forward-directional")), m_halfShadowArea(shadowArea / 2) {
	if (shadowMapSizeAsPowerOf2 != 0)
		setShadowData(ShadowData(Matrix4f().initOrthographic(-m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea), true, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));
}

ShadowCameraTransformer DirectionalLight::calcShadowCameraTransformer(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot) const {
	Vector3f resultPos = mainCameraPos + mainCameraRot.getForward() * getHalfShadowArea();
	Quaternion resultRot = getTransformer().getTransformedRot();

	float worldTexelSize = (getHalfShadowArea() * 2) / ((float)(1 << getShadowData().getShadowMapSizeAsPowerOf2()));

	Vector3f lightSpaceCameraPos = resultPos.rotate(resultRot.conjugate());

	lightSpaceCameraPos.setX(worldTexelSize * floor(lightSpaceCameraPos.getX() / worldTexelSize));
	lightSpaceCameraPos.setY(worldTexelSize * floor(lightSpaceCameraPos.getY() / worldTexelSize));

	resultPos = lightSpaceCameraPos.rotate(resultRot);

	return ShadowCameraTransformer(resultPos, resultRot);
}

PointLight::PointLight(const Vector3f& color, float intensity, const Attenuation& atten, const Shader& shader) : BaseLight(color, intensity, shader), m_attenuation(atten) {
	float a = m_attenuation.getExponent();
	float b = m_attenuation.getLinear();
	float c = m_attenuation.getConstant() - COLOR_DEPTH * intensity * color.max();

	m_range = (-b + sqrtf(b*b - 4 * a*c)) / (2 * a);
}

SpotLight::SpotLight(const Vector3f& color, float intensity, const Attenuation& atten, float viewAngle, int shadowMapSizeAsPowerOf2, float shadowSoftness, float lightBleedReductionAmount, float minVariance) : PointLight(color, intensity, atten), m_cutoff(cos(viewAngle / 2)) {
	if (shadowMapSizeAsPowerOf2 != 0)
		setShadowData(ShadowData(Matrix4f().initPerspective(viewAngle, 1, 0.1, getRange()), false, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));
}
