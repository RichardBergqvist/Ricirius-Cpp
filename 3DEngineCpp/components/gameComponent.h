#ifndef GAMECOMPONENT_H_INCLUDED
#define GAMECOMPONENT_H_INCLUDED

#include "transformer.h"
#include "gameObject.h"
#include "input.h"
class GraphicsEngine;
class Shader;

class GameComponent {
public:
	GameComponent() : m_parent(0) {}
	virtual ~GameComponent() {}

	virtual void processInput(const Input& input, float delta) {}
	virtual void update(float delta) {}
	virtual void render(const Shader& shader, const GraphicsEngine& graphicsEngine, const Camera& camera) const {}
	
	virtual void addToEngine(CoreEngine* engine) const {}
	
	inline Transformer* getTransformer() { return m_parent->getTransformer(); }
	inline const Transformer& getTransformer() const { return *m_parent->getTransformer(); }
	virtual void setParent(GameObject* parent) { m_parent = parent; }
	
private:
	GameObject* m_parent;

	GameComponent(const GameComponent& other) {}
	void operator=(const GameComponent& other) {}
};

#endif // GAMECOMPONENT_H_INCLUDED
