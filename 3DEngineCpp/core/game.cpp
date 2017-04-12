#include "game.h"
#include "../graphics/graphicsEngine.h"
#include <iostream>

void Game::processInput(const Input& input, float delta) {
	m_inputTimer.startInvocation();
	m_root.processInputAll(input, delta);
	m_inputTimer.stopInvocation();
}

void Game::update(float delta) {
	m_updateTimer.startInvocation();
	m_root.updateAll(delta);
	m_updateTimer.stopInvocation();
}

void Game::render(GraphicsEngine* graphicsEngine) {
	graphicsEngine->render(m_root);
}