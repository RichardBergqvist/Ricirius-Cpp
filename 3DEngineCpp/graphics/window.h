#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <string>
#include "../components/input.h"

class Window {
public:
	Window(int width, int height, const std::string& title);
	virtual ~Window();

	void update();
	void swapBuffers();
	void bindAsRenderTarget() const;

	inline bool isCloseRequested() const { return m_isCloseRequested; }
	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	inline float getAspect()  const { return (float) m_width / (float) m_height; }
	inline const std::string& getTitle() const { return m_title; }
	inline Vector2f getCenter() const { return Vector2f((float)m_width / 2.0f, (float)m_height / 2.0f); }
	inline SDL_Window* getSDLWindow() { return m_window; }
	inline const Input& getInput() const { return m_input; }

	void setFullScreen(bool value);
protected:
private:
	int m_width;
	int m_height;
	std::string m_title;
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	Input m_input;
	bool m_isCloseRequested;

	Window(const Window& other) : m_input(this) {}
	void operator=(const Window& other) {}
};

#endif