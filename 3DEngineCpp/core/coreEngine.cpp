#include "coreEngine.h"
#include "../util/time.h"
#include "../graphics/window.h"
#include "../components/input.h"
#include "../util/util.h"
#include "../core/game.h"

#include <stdio.h>

CoreEngine::CoreEngine(int width, int height, double frameRate, Game* game) : m_isRunning(false), m_width(width), m_height(height), m_frameTime(1 / frameRate), m_game(game), m_graphicsEngine(NULL) {
	m_game->setEngine(this);
}
	
CoreEngine::~CoreEngine() {
	if (m_window) delete m_window;
	if (m_graphicsEngine) delete m_graphicsEngine;
}

void CoreEngine::createWindow(const std::string& title) {
	m_window = new Window(m_width, m_height, title);
	m_graphicsEngine = new GraphicsEngine(*m_window);
}

void CoreEngine::start() {
	if(m_isRunning)
		return;
		
	run();
}

void CoreEngine::stop() {
	if(!m_isRunning)
		return;
		
	m_isRunning = false;
}

void CoreEngine::run() {
	m_isRunning = true;

	m_game->init(*m_window);

	double lastTime = Time::getTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	int frames = 0;

	while (m_isRunning) {
		bool render = false;

		double startTime = Time::getTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0) {
			printf("%f ms\n", 1000.0 / ((double) frames));
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime) {
			render = true;

			if (m_window->isCloseRequested())
				stop();

			m_window->update();

			m_game->processInput(m_window->getInput(), (float) m_frameTime);
			m_game->update((float)m_frameTime);

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			m_game->render(m_graphicsEngine, *m_mainCamera);
			m_window->swapBuffers();
			frames++;
		}
		else
		{
			Util::sleep(1);
		}
	}
}