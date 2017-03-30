#include "coreEngine.h"
#include "../util/time.h"
#include "../graphics/window.h"
#include "../components/input.h"
#include "../util/graphicsUtil.h"
#include "../util/util.h"
#include "../components/game.h"

#include <stdio.h>

CoreEngine::CoreEngine(int width, int height, double frameRate, Game* game) :
	m_isRunning(false),
	m_width(width),
	m_height(height),
	m_frameTime(1.0/frameRate),
	m_game(game),
	m_graphicsEngine(NULL)
{
	m_game->setEngine(this);
}
	
CoreEngine::~CoreEngine() {
	Window::dispose();
	if(m_graphicsEngine) delete m_graphicsEngine;
}

void CoreEngine::createWindow(const std::string& title) {
	Window::create(m_width, m_height, title);
	GraphicsUtil::initGraphics();
	m_graphicsEngine = new GraphicsEngine();
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

	m_game->init();

	double lastTime = Time::getTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	int frames = 0;

	while(m_isRunning) {
		bool render = false;

		double startTime = Time::getTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if(frameCounter >= 1.0) {
			printf("%i\n",frames);
			frames = 0;
			frameCounter = 0;
		}

		while(unprocessedTime > m_frameTime) {
			render = true;

			if(Window::isCloseRequested())
				stop();

			Input::update();

			m_game->input((float)m_frameTime);
			m_game->update((float)m_frameTime);

			unprocessedTime -= m_frameTime;
		}

		if(render) {
			m_game->render(m_graphicsEngine);
			Window::render();
			frames++;
		} else {
			Util::sleep(1);
		}
	}
}

