#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include "../components/camera.h"
#include "lighting.h"
#include "mappedValues.h"
#include "material.h"
#include "model.h"
#include "window.h"
#include <vector>
#include <map>
class GameObject;

class GraphicsEngine : public MappedValues {
public:
	GraphicsEngine(const Window& window);
	//virtual ~GraphicsEngine() {}

	void render(const GameObject& object, const Camera& mainCamera);

	inline void addLight(const BaseLight& light) { m_lights.push_back(&light); }
	//inline void addCamera(const Camera& camera) { m_mainCamera = &camera; }

	virtual void updateUniformStruct(const Transformer& transform, const Material& material, const Shader& shader, const std::string& uniformName, const std::string& uniformType) const {
		throw uniformType + " is not supported by the rendering engine";
	}

	//inline const Camera& getMainCamera() const { return *m_mainCamera; }
	inline const BaseLight& getActiveLight() const { return *m_activeLight; }
	inline unsigned int getSamplerSlot(const std::string& samplerName) const { return m_samplerMap.find(samplerName)->second; }
	inline const Matrix4f& getLightMatrix() const { return m_lightMatrix; }

protected:
	inline void setSamplerSlot(const std::string& name, unsigned int value) { m_samplerMap[name] = value; }
private:
	static const int NUM_SHADOW_MAPS = 10;
	static const Matrix4f BIAS_MATRIX;

	Transformer m_planeTransformer;
	Model m_plane;

	const Window* m_window;
	Texture m_tempTarget;
	Material m_planeMaterial;
	Texture m_shadowMaps[NUM_SHADOW_MAPS];
	Texture m_shadowMapTempTargets[NUM_SHADOW_MAPS];

	Shader m_defaultShader;
	Shader m_shadowMapShader;
	Shader m_nullFilter;
	Shader m_gausBlurFilter;
	Shader m_fxaaFilter;
	Matrix4f m_lightMatrix;

	Transformer m_altCameraTransformer;
	Camera m_altCamera;
	const BaseLight* m_activeLight;
	std::vector<const BaseLight*> m_lights;
	std::map<std::string, unsigned int> m_samplerMap;

	void blurShadowMap(int shadowMapIndex, float blurAmount);
	void applyFilter(const Shader& filter, const Texture& source, const Texture* dest);

	//GraphicsEngine(const GraphicsEngine& other) {}
	//void operator=(const GraphicsEngine& other) {}
};

#endif // GRAPHICSENGINE_H