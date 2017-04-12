#ifndef LIGHTING_H
#define LIGHTING_H

#include "../util/math3d.h"
#include "../components/entityComponent.h"
#include "shader.h"

class CoreEngine;

class ShadowCameraTransformer {
public:
	ShadowCameraTransformer(const Vector3f& pos, const Quaternion& rot) : m_pos(pos), m_rot(rot) {}

	inline const Vector3f& getPos() const { return m_pos; }
	inline const Quaternion& getRot() const { return m_rot; }
private:
	Vector3f m_pos;
	Quaternion m_rot;
};

class ShadowData {
public:
	ShadowData(const Matrix4f& projection = Matrix4f().initIdentity(), bool flipFaces = false, int shadowMapSizeAsPowerOf2 = 0, float shadowSoftness = 1.0f, float lightBleedReductionAmount = 0.2f, float minVariance = 0.00002f) : m_projection(projection), m_flipFaces(flipFaces), m_shadowMapSizeAsPowerOf2(shadowMapSizeAsPowerOf2), m_shadowSoftness(shadowSoftness), m_lightBleedReductionAmount(lightBleedReductionAmount), m_minVariance(minVariance) {}

	inline const Matrix4f& getProjection() const { return m_projection; }
	inline bool getFlipFaces() const { return m_flipFaces; }
	inline int getShadowMapSizeAsPowerOf2() const { return m_shadowMapSizeAsPowerOf2; }
	inline float getShadowSoftness() const { return m_shadowSoftness; }
	inline float getMinVariance() const { return m_minVariance; }
	inline float getLightBleedReductionAmount() const { return m_lightBleedReductionAmount; }
protected:
private:
	Matrix4f m_projection;
	bool m_flipFaces;
	int m_shadowMapSizeAsPowerOf2;
	float m_shadowSoftness;
	float m_lightBleedReductionAmount;
	float m_minVariance;
};

class BaseLight : public EntityComponent {
public:
	BaseLight(const Vector3f& color, float intensity, const Shader& shader) : m_color(color), m_intensity(intensity), m_shader(shader), m_shadowData(ShadowData()) {}

	virtual ShadowCameraTransformer calcShadowCameraTransformer(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot) const;
	virtual void addToEngine(CoreEngine* engine) const;	
	
	inline const Vector3f& getColor() const { return m_color; }
	inline const float getIntensity() const { return m_intensity; }
	inline const Shader& getShader() const { return m_shader; }
	inline const ShadowData& getShadowData() const { return m_shadowData; }
protected:
	inline void setShadowData(const ShadowData& shadowData) { m_shadowData = shadowData; }
private:
	Vector3f m_color;
	float m_intensity;
	Shader m_shader;
	ShadowData m_shadowData;
};

class DirectionalLight : public BaseLight {
public:
	DirectionalLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0, int shadowMapSizeAsPowerOf2 = 0, float shadowArea = 80, float shadowSoftness = 1, float lightBleedReductionAmount = 0.2F, float minVariance = 0.00002F);

	virtual ShadowCameraTransformer calcShadowCameraTransformer(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot) const;

	inline float getHalfShadowArea() const { return m_halfShadowArea; }
private:
	float m_halfShadowArea;
};

class Attenuation {
public:
	Attenuation(float constant = 0, float linear = 0, float exponent = 1) : m_constant(constant), m_linear(linear), m_exponent(exponent) {}

	inline float getConstant() const { return m_constant; }
	inline float getLinear() const { return m_linear; }
	inline float getExponent() const { return m_exponent; }
private:
	float m_constant;
	float m_linear;
	float m_exponent;
};

class PointLight : public BaseLight {
public:
	PointLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0, const Attenuation& atten = Attenuation(), const Shader& shader = Shader("forward-point"));

	inline const Attenuation& getAttenuation() const { return m_attenuation; }
	inline const float getRange() const { return m_range; }
private:
	Attenuation m_attenuation;
	float m_range;
};

class SpotLight : public PointLight {
public:
	SpotLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0, const Attenuation& atten = Attenuation(), float viewAngle = toRadians(170.0), int shadowMapSizeAsPowerOf2 = 0, float shadowSoftness = 1, float lightBleedReductionAmount = 0.2F, float minVariance = 0.00002F);

	inline float getCutoff() const { return m_cutoff; }
private:
	float m_cutoff;
};

#endif
