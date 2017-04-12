#include "coreEngine.h"
#include "../util/time.h"
#include "../graphics/window.h"
#include "../components/input.h"
#include "../util/util.h"
#include "../core/game.h"

#include <stdio.h>

CoreEngine::CoreEngine(double frameRate, Window* window, GraphicsEngine* graphicsEngine, Game* game) : m_isRunning(false), m_frameTime(1 / frameRate), m_window(window), m_graphicsEngine(graphicsEngine), m_game(game) {
	m_game->setEngine(this);
	m_game->init(*m_window);
}

void CoreEngine::start() {
	if(m_isRunning)
		return;
		
	m_isRunning = true;

	double lastTime = Time::getTime();
	double frameCounter = 0;
	double unprocessedTime = 0; 
	int frames = 0; 

	while (m_isRunning) {
		bool render = false;

		double startTime = Time::getTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0) {
			printf("%f ms\n", 1000.0 / ((double)frames));
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime) {
			m_window->update();

			if (m_window->isCloseRequested()) {
				stop();
			}

			m_game->processInput(m_window->getInput(), (float) m_frameTime);
			m_game->update((float) m_frameTime);

			render = true;

			unprocessedTime -= m_frameTime;
		}

		if (render) {
			m_game->render(m_graphicsEngine);
			m_window->swapBuffers();
			frames++;
		} else {
			Util::sleep(1);
		}
	}
}

void CoreEngine::stop() {
	m_isRunning = false;
}