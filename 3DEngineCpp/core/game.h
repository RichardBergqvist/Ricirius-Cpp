#ifndef MYGAME_H
#define MYGAME_H

#include "../components/gameObject.h"
#include "coreEngine.h"
#include "../components/camera.h"

class Game {
public:
	Game() {}
	virtual ~Game() {}

	virtual void init(const Window& window) {}
	void processInput(const Input& input, float delta);
	void update(float delta);
	void render(GraphicsEngine* graphicsEngine, const Camera& camera);
	
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
