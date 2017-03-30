#include "../graphics/lighting.h"
#include "../graphics/graphicsEngine.h"
#include "../graphics/shader.h"
#include "../core/coreEngine.h"

#define COLOR_DEPTH 256

void BaseLight::addToEngine(CoreEngine* engine) {
	engine->getGraphicsEngine()->addLight(this);
}

BaseLight::~BaseLight() {
	if (m_shader) delete m_shader;
	if (m_shadowData) delete m_shadowData;
}

void BaseLight::setShader(Shader* shader) {
	if (m_shader) delete m_shader;
	m_shader = shader; 
}

void BaseLight::setShadowData(ShadowData* shadowData) {
	if (m_shadowData) delete m_shadowData;
	m_shadowData = shadowData;
}

ShadowCameraTransformer BaseLight::calcShadowCameraTransformer(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot) {
	ShadowCameraTransformer result;
	result.pos = getTransformer().getTransformedPos();
	result.rot = getTransformer().getTransformedRot();
	return result;
}

DirectionalLight::DirectionalLight(const Vector3f& color, float intensity, int shadowMapSizeAsPowerOf2, float shadowArea, float shadowSoftness, float lightBleedReductionAmount, float minVariance) : BaseLight(color, intensity) {
	setShader(new Shader("forward-directional"));

	this->halfShadowArea = shadowArea / 2;
	if (shadowMapSizeAsPowerOf2 != 0) {
		setShadowData(new ShadowData(Matrix4f().initOrthographic(-halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea), true, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));
	}
}

ShadowCameraTransformer DirectionalLight::calcShadowCameraTransformer(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot) {
	ShadowCameraTransformer result;
	result.pos = mainCameraPos + mainCameraRot.getForward() * halfShadowArea;
	result.rot = getTransformer().getTransformedRot();
	
	float worldTexelSize = (halfShadowArea * 2) / ((float) (1 << getShadowData()->getShadowMapSizeAsPowerOf2()));

	Vector3f lightSpaceCameraPos = result.pos.rotate(result.rot.conjugate());

	lightSpaceCameraPos.setX(worldTexelSize * floor(lightSpaceCameraPos.getX() / worldTexelSize));
	lightSpaceCameraPos.setY(worldTexelSize * floor(lightSpaceCameraPos.getY() / worldTexelSize));

	result.pos = lightSpaceCameraPos.rotate(result.rot);

	return result;
}

PointLight::PointLight(const Vector3f& color, float intensity, const Attenuation& atten) : BaseLight(color, intensity), atten(atten) {
	float a = atten.exponent;
	float b = atten.linear;
	float c = atten.constant - COLOR_DEPTH * intensity * color.max();
	
	range = (-b + sqrtf(b*b - 4*a*c))/(2*a);

	setShader(new Shader("forward-point"));
}

SpotLight::SpotLight(const Vector3f& color, float intensity, const Attenuation& atten, float viewAngle, int shadowMapSizeAsPowerOf2, float shadowSoftness, float lightBleedReductionAmount, float minVariance) : PointLight(color, intensity, atten), viewAngle(cos(viewAngle / 2)) {
	setShader(new Shader("forward-spot"));

	if (shadowMapSizeAsPowerOf2 != 0) {
		setShadowData(new ShadowData(Matrix4f().initPerspective(viewAngle, 1, 0.1, this->range), false, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));
	}
}
