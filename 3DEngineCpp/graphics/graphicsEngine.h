#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include "../components/camera.h"
#include "../graphics/lighting.h"
#include "../core/mappedValues.h"
#include "../graphics/material.h"
#include <vector>
#include <map>
class GameObject;
class Model;

class GraphicsEngine : public MappedValues {
public:
	GraphicsEngine();
	
	void render(GameObject* object);
	
	inline Camera& getMainCamera() { return *m_mainCamera; }
	inline BaseLight* getActiveLight() { return m_activeLight; }
	
	inline void addLight(BaseLight* light) { m_lights.push_back(light); }
	inline void addCamera(Camera* camera) { m_mainCamera = camera; }
	
	inline unsigned int getSamplerSlot(const std::string& samplerName) { return m_samplerMap[samplerName]; }
	inline Matrix4f getLightMatrix() { return m_lightMatrix;  }
	
	virtual void updateUniformStruct(const Transformer& transform, const Material& material, Shader* shader, 
		const std::string& uniformName, const std::string& uniformType) {
		throw uniformType + " is not supported by the rendering engine";
	}
	
	virtual ~GraphicsEngine();
protected:
private:
	static const int s_numShadowMaps = 10;
	static const Matrix4f s_biasMatrix;

	GraphicsEngine(const GraphicsEngine& other) {}
	void operator=(const GraphicsEngine& other) {}
	void blurShadowMap(int shadowMapIndex, float blurAmount);
	void applyFilter(Shader* filter, Texture* source, Texture* dest);

	Camera* m_mainCamera;
	Camera* m_altCamera;
	GameObject* m_altCameraObject;

	Material* m_planeMaterial;
	Transformer m_planeTransformer;
	Model* m_plane;
	Texture* m_tempTarget;

	Texture* m_shadowMaps[s_numShadowMaps];
	Texture* m_shadowMapTempTargets[s_numShadowMaps];

	BaseLight* m_activeLight;
	Shader* m_defaultShader;
	Shader* m_shadowMapShader;
	Shader* m_nullFilter;
	Shader* m_gausBlurFilter;
	Matrix4f m_lightMatrix;

	std::vector<BaseLight*> m_lights;
	std::map<std::string, unsigned int> m_samplerMap;
};

#endif // RENDERINGENGINE_H
