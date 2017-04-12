#ifndef COREENGINE_H
#define COREENGINE_H

#include "../graphics/graphicsEngine.h"
#include <string>
class Game;

class CoreEngine {
public:
	CoreEngine(double frameRate, Window* window, GraphicsEngine* graphicsEngine, Game* game);
	
	void start();
	void stop();

	inline GraphicsEngine* getGraphicsEngine() { return m_graphicsEngine; }
protected:
private:
	bool m_isRunning;
	double m_frameTime;
	Window* m_window;
	GraphicsEngine* m_graphicsEngine;
	Game* m_game;
};

#endif // COREENGINE_H
