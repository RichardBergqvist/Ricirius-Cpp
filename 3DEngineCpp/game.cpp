#include "game.h"
#include "graphicsEngine.h"
#include <iostream>

void Game::input(float delta) {
	m_root.inputAll(delta);
}

void Game::update(float delta) {
	m_root.updateAll(delta);
}

void Game::render(GraphicsEngine* graphicsEngine) {
	graphicsEngine->render(&m_root);
}
