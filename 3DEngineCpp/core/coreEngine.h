#ifndef COREENGINE_H
#define COREENGINE_H

#include "../graphics/graphicsEngine.h"
#include <string>
class Game;

class CoreEngine {
public:
	CoreEngine(int width, int height, double frameRate, Game* game);
	virtual ~CoreEngine();
	
	void createWindow(const std::string& title);
	
	void start();
	void stop();
	
	inline GraphicsEngine* getGraphicsEngine() { return m_graphicsEngine; }
	void setCamera(const Camera& camera) { m_mainCamera = &camera; }
protected:
private:
	bool m_isRunning;
	int m_width;
	int m_height;
	double m_frameTime;
	Game* m_game;
	GraphicsEngine* m_graphicsEngine;
	Window* m_window;
	const Camera* m_mainCamera;

	void run();

	CoreEngine(const CoreEngine& other) {}
	void operator=(const CoreEngine& other) {}
};

#endif // COREENGINE_H
