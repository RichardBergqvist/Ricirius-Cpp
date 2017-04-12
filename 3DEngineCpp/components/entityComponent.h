#ifndef ENTITYCOMPONENT_H_INCLUDED
#define ENTITYCOMPONENT_H_INCLUDED

#include "transformer.h"
#include "entity.h"
#include "input.h"
class GraphicsEngine;
class Shader;

class EntityComponent {
public:
	EntityComponent() : m_parent(0) {}
	virtual ~EntityComponent() {}

	virtual void processInput(const Input& input, float delta) {}
	virtual void update(float delta) {}
	virtual void render(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const {}
	
	virtual void addToEngine(CoreEngine* engine) const {}
	
	inline Transformer* getTransformer() { return m_parent->getTransformer(); }
	inline const Transformer& getTransformer() const { return *m_parent->getTransformer(); }
	virtual void setParent(Entity* parent) { m_parent = parent; }
	
private:
	Entity* m_parent;

	EntityComponent(const EntityComponent& other) {}
	void operator=(const EntityComponent& other) {}
};

#endif // ENTITYCOMPONENT_H_INCLUDED
