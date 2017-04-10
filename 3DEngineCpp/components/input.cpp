#include "input.h"
#include "../graphics/window.h"
#include <SDL2/SDL.h>
#include <cstring>

Input::Input(Window* window) : m_mouseX(0), m_mouseY(0), m_window(window) {
	memset(m_inputs, 0, NUM_KEYS * sizeof(bool));
	memset(m_downKeys, 0, NUM_KEYS * sizeof(bool));
	memset(m_upKeys, 0, NUM_KEYS * sizeof(bool));

	memset(m_mouseInput, 0, NUM_MOUSEBUTTONS * sizeof(bool));
	memset(m_downMouse, 0, NUM_MOUSEBUTTONS * sizeof(bool));
	memset(m_upMouse, 0, NUM_MOUSEBUTTONS * sizeof(bool));
}

void Input::setCursor(bool visible) const {
	if (visible)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

void Input::setMousePosition(const Vector2f& pos) const {
	SDL_WarpMouseInWindow(m_window->getSDLWindow(), (int) pos.getX(), (int) pos.getY());
}