#ifndef MYGAME_H
#define MYGAME_H

#include "gameObject.h"
#include "../core/coreEngine.h"

class Game {
public:
	Game() {}
	virtual ~Game() {}

	virtual void init() {}
	void input(float delta);
	void update(float delta);
	void render(GraphicsEngine* graphicsEngine);
	
	inline GameObject& getRoot() { return m_root; }
	
	inline void setEngine(CoreEngine* engine) { m_root.setEngine(engine); }
protected:
	void addToScene(GameObject* child) { m_root.addChild(child); }
private:
	Game(Game& game) {}
	void operator=(Game& game) {}
	
	GameObject m_root;
};

#endif
