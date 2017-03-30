#ifndef LIGHTING_H
#define LIGHTING_H

#include "../util/math3d.h"
#include "../components/gameComponent.h"

class CoreEngine;

struct ShadowCameraTransformer {
	Vector3f pos;
	Quaternion rot;
};

class ShadowData {
public:
	ShadowData(const Matrix4f& projection, bool flipFaces, int shadowMapSizeAsPowerOf2, float shadowSoftness = 1.0F, float lightBleedReductionAmount = 0.2F, float minVariance = 0.00002F) : m_projection(projection), m_flipFaces(flipFaces), m_shadowMapSizeAsPowerOf2(shadowMapSizeAsPowerOf2), m_shadowSoftness(shadowSoftness), m_lightBleedReductionAmount(lightBleedReductionAmount), m_varianceMin(minVariance) {}

	inline Matrix4f getProjection() { return m_projection; }
	inline bool getFlipFaces() { return m_flipFaces; }
	inline int getShadowMapSizeAsPowerOf2() { return m_shadowMapSizeAsPowerOf2;  };
	inline float getShadowSoftness() { return m_shadowSoftness; }
	inline float getLightBleedReductionAmount() { return m_lightBleedReductionAmount; }
	inline float getVarianceMin() { return m_varianceMin; }
protected:
private:
	Matrix4f m_projection;
	bool m_flipFaces;
	int m_shadowMapSizeAsPowerOf2;
	float m_shadowSoftness;
	float m_lightBleedReductionAmount;
	float m_varianceMin;
};

class BaseLight : public GameComponent {
public:
	Vector3f color;
	float intensity;
	
	BaseLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0) : color(color), intensity(intensity), m_shader(0), m_shadowData(0) {}
	
	virtual ~BaseLight();

	virtual ShadowCameraTransformer calcShadowCameraTransformer(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot);
	
	virtual void addToEngine(CoreEngine* engine);	
	inline Shader* getShader() { return m_shader; }
	inline ShadowData* getShadowData() { return m_shadowData;  }
	
protected:
	void setShader(Shader* shader);
	void setShadowData(ShadowData* shadowData);
private:
	BaseLight(BaseLight& other) {}
	void operator=(BaseLight& other) {}

	Shader* m_shader;
	ShadowData* m_shadowData;
};

struct DirectionalLight : public BaseLight {
	DirectionalLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0, int shadowMapSizeAsPowerOf2 = 0, float shadowArea = 80.0F, float shadowSoftness = 1.0F, float lightBleedReductionAmount = 0.2F, float minVariance = 0.00002F);

	float halfShadowArea;
	virtual ShadowCameraTransformer calcShadowCameraTransformer(const Vector3f& mainCameraPos, const Quaternion& mainCameraRot);
};

struct Attenuation {
	float constant;
	float linear;
	float exponent;

	Attenuation(float constant = 0, float linear = 0, float exponent = 1) :
		constant(constant),
		linear(linear),
		exponent(exponent) {}
};

struct PointLight : public BaseLight {
	Attenuation atten;
	float range;

	PointLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0, const Attenuation& atten = Attenuation());
};

struct SpotLight : public PointLight {
	float viewAngle;

	SpotLight(const Vector3f& color = Vector3f(0,0,0), float intensity = 0, const Attenuation& atten = Attenuation(), float viewAngle = toRadians(170.0), int shadowMapSizeAsPowerOf2 = 0, float shadowSoftness = 1.0F, float lightBleedReductionAmount = 0.2F, float minVariance = 0.00002F);
};

#endif
