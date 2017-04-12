#ifndef ENTITYOBJECT_H
#define ENTITYOBJECT_H

#include <vector>
#include "transformer.h"
#include "input.h"
class Camera;
class CoreEngine;
class EntityComponent;
class Shader;
class GraphicsEngine;

class Entity {
public:
	Entity(const Vector3f& pos = Vector3f(0, 0, 0), const Quaternion& rot = Quaternion(0, 0, 0, 1), float scale = 1) : m_transformer(pos, rot, scale), m_coreEngine(0) {}
	virtual ~Entity();
	
	Entity* addChild(Entity* child);
	Entity* addComponent(EntityComponent* component);
	
	void processInputAll(const Input& input, float delta);
	void updateAll(float delta);
	void renderAll(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const;
	
	std::vector<Entity*> getAllAttached();
	
	inline Transformer* getTransformer() { return &m_transformer; }
	void setEngine(CoreEngine* engine);
protected:
private:
	std::vector<Entity*> m_children;
	std::vector<EntityComponent*> m_components;
	Transformer m_transformer;
	CoreEngine* m_coreEngine;

	void processInput(const Input& input, float delta);
	void update(float delta);
	void render(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const;

	Entity(const Entity& other) {}
	void operator=(const Entity& other) {}
};

#endif // ENTITYOBJECT_H
