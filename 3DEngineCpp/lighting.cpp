#include "lighting.h"
#include "graphicsEngine.h"
#include "shader.h"
#include "coreEngine.h"

#define COLOR_DEPTH 256

void BaseLight::addToEngine(CoreEngine* engine) {
	engine->getGraphicsEngine()->addLight(this);
}

BaseLight::~BaseLight() {
	if(m_shader) delete m_shader;
}

void BaseLight::setShader(Shader* shader) {
	if(m_shader) delete m_shader;
	m_shader = shader; 
}

DirectionalLight::DirectionalLight(const Vector3f& color, float intensity) : BaseLight(color, intensity) {
	setShader(new Shader("forward-directional"));
}

PointLight::PointLight(const Vector3f& color, float intensity, const Attenuation& atten) : BaseLight(color, intensity), atten(atten) {
	float a = atten.exponent;
	float b = atten.linear;
	float c = atten.constant - COLOR_DEPTH * intensity * color.max();
	
	range = (-b + sqrtf(b*b - 4*a*c))/(2*a);

	setShader(new Shader("forward-point"));
}

SpotLight::SpotLight(const Vector3f& color, float intensity, const Attenuation& atten, float cutoff) : PointLight(color, intensity, atten), cutoff(cutoff) {
	setShader(new Shader("forward-spot"));
}
