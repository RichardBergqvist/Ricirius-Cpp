#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "transformer.h"
class CoreEngine;
class GameComponent;
class Shader;
class GraphicsEngine;

class GameObject {
public:
	GameObject() { m_coreEngine = 0; }
	virtual ~GameObject();
	
	GameObject* addChild(GameObject* child);
	GameObject* addComponent(GameComponent* component);
	
	void inputAll(float delta);
	void updateAll(float delta);
	void renderAll(Shader* shader, GraphicsEngine* graphicsEngine);
	
	std::vector<GameObject*> getAllAttached();
	
	inline Transformer& getTransformer() { return m_transformer; }
	void setEngine(CoreEngine* engine);
protected:
private:
	void input(float delta);
	void update(float delta);
	void render(Shader* shader, GraphicsEngine* graphicsEngine);

	std::vector<GameObject*> m_children;
	std::vector<GameComponent*> m_components;
	Transformer m_transformer;
	CoreEngine* m_coreEngine;
};

#endif // GAMEOBJECT_H
