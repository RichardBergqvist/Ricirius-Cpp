#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include "camera.h"
#include "lighting.h"
#include "mappedValues.h"
#include "material.h"
#include <vector>
#include <map>
class GameObject;

class GraphicsEngine : public MappedValues {
public:
	GraphicsEngine();
	
	void render(GameObject* object);
	
	inline Camera& getMainCamera() { return *m_mainCamera; }
	inline BaseLight* getActiveLight() { return m_activeLight; }
	
	inline void addLight(BaseLight* light) { m_lights.push_back(light); }
	inline void addCamera(Camera* camera) { m_mainCamera = camera; }
	
	inline unsigned int getSamplerSlot(const std::string& samplerName) { return m_samplerMap[samplerName]; }
	
	virtual void updateUniformStruct(const Transformer& transform, const Material& material, Shader* shader, 
		const std::string& uniformName, const std::string& uniformType) {
		throw uniformType + " is not supported by the rendering engine";
	}
	
	virtual ~GraphicsEngine();
protected:
private:
	GraphicsEngine(const GraphicsEngine& other) {}
	void operator=(const GraphicsEngine& other) {}
	
	Camera* m_mainCamera;
	BaseLight* m_activeLight;
	Shader* m_defaultShader;
	std::vector<BaseLight*> m_lights;
	std::map<std::string, unsigned int> m_samplerMap;
};

#endif // RENDERINGENGINE_H
