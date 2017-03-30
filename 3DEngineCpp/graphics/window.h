#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <string>

class Window {
public:
	static void create(int width, int height, const std::string& title);
	static void render();
	static void dispose();
	static void bindAsRenderTarget();

	static bool isCloseRequested();
	static int getWidth();
	static int getHeight();
	static float getAspect();
	static const std::string& getTitle();

	static void setFullScreen(bool value);
protected:
private:
	static int s_width;
	static int s_height;
	static std::string s_title;
};

#endif
