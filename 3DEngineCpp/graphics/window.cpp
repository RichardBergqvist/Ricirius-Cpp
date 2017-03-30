#include "window.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "../util/sdl_backend.h"

int Window::s_width = 0;
int Window::s_height = 0;
std::string Window::s_title = "Title";

void Window::create(int width, int height, const std::string& title) {
	Window::s_width = width;
	Window::s_height = height;
	Window::s_title = title;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	SDLCreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, false);

	GLenum res = glewInit();
	if(res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
	}
}

void Window::render() {
	SDLSwapBuffers();
}

void Window::dispose() {
	SDLDestroyWindow();
	SDL_Quit();
}

void Window::bindAsRenderTarget() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, getWidth(), getHeight());
}

bool Window::isCloseRequested() {
	return SDLGetIsCloseRequested();
}

void Window::setFullScreen(bool value) {
	SDLSetWindowFullscreen(value);
}

int Window::getWidth() {
	return s_width;
}

int Window::getHeight() {
	return s_height;
}

float Window::getAspect() {
	return (float)getWidth()/(float)getHeight();
}

const std::string& Window::getTitle() {
	return s_title;
}
