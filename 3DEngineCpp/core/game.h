#ifndef MYGAME_H
#define MYGAME_H

#include "../components/gameObject.h"
#include "coreEngine.h"
#include "../util/profiling.h"

class Game {
public:
	Game() {}
	virtual ~Game() {}

	virtual void init(const Window& window) {}
	void processInput(const Input& input, float delta);
	void update(float delta);
	void render(GraphicsEngine* graphicsEngine);
	
	inline double displayInputTime(double dividend) { return m_inputTimer.displayAndReset("Input Time: ", dividend); }
	inline double displayUpdateTime(double dividend) { return m_updateTimer.displayAndReset("Update Time: ", dividend); }
	
	inline void setEngine(CoreEngine* engine) { m_root.setEngine(engine); }
protected:
	void addToScene(GameObject* child) { m_root.addChild(child); }
private:
	Game(Game& game) {}
	void operator=(Game& game) {}
	
	ProfileTimer m_updateTimer;
	ProfileTimer m_inputTimer;
	GameObject m_root;
};

#endif
