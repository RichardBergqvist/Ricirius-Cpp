#ifndef GAMECOMPONENT_H_INCLUDED
#define GAMECOMPONENT_H_INCLUDED

#include "transformer.h"
#include "gameObject.h"
class GraphicsEngine;
class Shader;

class GameComponent {
public:
	virtual ~GameComponent() {}

	virtual void input(float delta) {}
	virtual void update(float delta) {}
	virtual void render(Shader* shader, GraphicsEngine* graphicsEngine) {}
	
	virtual void addToEngine(CoreEngine* engine) { }
	
	inline void setParent(GameObject* parent) { m_parent = parent; }
	inline Transformer& getTransform() { return m_parent->getTransform(); }
	inline const Transformer& getTransform() const { return m_parent->getTransform(); }
	
private:
	GameObject* m_parent;
};

#endif // GAMECOMPONENT_H_INCLUDED
