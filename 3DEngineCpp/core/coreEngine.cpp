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

	ProfileTimer sleepTimer;
	ProfileTimer swapBufferTimer;
	ProfileTimer windowUpdateTimer;
	while (m_isRunning) {
		bool render = false;

		double startTime = Time::getTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0) {
			double totalTime = ((1000.0 * frameCounter) / ((double) frames));
			double totalMeasuredTime = 0.0;

			totalMeasuredTime += m_game->displayInputTime((double) frames);
			totalMeasuredTime += m_game->displayUpdateTime((double) frames);
			totalMeasuredTime += m_graphicsEngine->displayRenderTime((double) frames);
			totalMeasuredTime += sleepTimer.displayAndReset("Sleep Time: ", (double) frames);
			totalMeasuredTime += windowUpdateTimer.displayAndReset("Window Update Time: ", (double) frames);
			totalMeasuredTime += swapBufferTimer.displayAndReset("Buffer Swap Time: ", (double) frames);
			totalMeasuredTime += m_graphicsEngine->displayWindowSyncTime((double) frames);
			
			printf("Other Time: %f ms\n", (totalTime - totalMeasuredTime));
			printf("Total Time: %f ms\n\n", totalTime);
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime) {
			windowUpdateTimer.startInvocation();
			m_window->update();

			if (m_window->isCloseRequested()) {
				stop();
			}
			windowUpdateTimer.stopInvocation();

			m_game->processInput(m_window->getInput(), (float) m_frameTime);
			m_game->update((float) m_frameTime);

			render = true;

			unprocessedTime -= m_frameTime;
		}

		if (render) {
			m_game->render(m_graphicsEngine);
			swapBufferTimer.startInvocation();
			m_window->swapBuffers();
			swapBufferTimer.stopInvocation();
			frames++;
		} else {
			sleepTimer.startInvocation();
			Util::sleep(1);
			sleepTimer.stopInvocation();
		}
	}
}

void CoreEngine::stop() {
	m_isRunning = false;
}