#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "../util/transformer.h"
#include "input.h"
class Camera;
class CoreEngine;
class GameComponent;
class Shader;
class GraphicsEngine;

class GameObject {
public:
	GameObject(const Vector3f& pos = Vector3f(0, 0, 0), const Quaternion& rot = Quaternion(0, 0, 0, 1), float scale = 1) : m_transformer(pos, rot, scale), m_coreEngine(0) {}
	virtual ~GameObject();
	
	GameObject* addChild(GameObject* child);
	GameObject* addComponent(GameComponent* component);
	
	void processInputAll(const Input& input, float delta);
	void updateAll(float delta);
	void renderAll(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const;
	
	std::vector<GameObject*> getAllAttached();
	
	inline Transformer* getTransformer() { return &m_transformer; }
	void setEngine(CoreEngine* engine);
protected:
private:
	std::vector<GameObject*> m_children;
	std::vector<GameComponent*> m_components;
	Transformer m_transformer;
	CoreEngine* m_coreEngine;

	void processInput(const Input& input, float delta);
	void update(float delta);
	void render(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const;

	GameObject(const GameObject& other) {}
	void operator=(const GameObject& other) {}
};

#endif // GAMEOBJECT_H
