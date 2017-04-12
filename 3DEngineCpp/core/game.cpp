#include "game.h"
#include "../graphics/graphicsEngine.h"
#include <iostream>

void Game::processInput(const Input& input, float delta) {
	m_root.processInputAll(input, delta);
}

void Game::update(float delta) {
	m_root.updateAll(delta);
}

void Game::render(GraphicsEngine* graphicsEngine) {
	graphicsEngine->render(m_root);
}