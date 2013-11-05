#ifndef OS_LAYER_NAMESPACE
#define OS_LAYER_NAMESPACE

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <iostream>

#include "CommonClasses.h"
#include "Config.h"
#include "CurrentUpdateState.h"
#include "ErrorHandler.h"

namespace OS
{
	void setupRC();
	void handleEvents();
	void handleSDLEvent(SDL_Event const &sdlEvent);
	void swapBuffers();

	void setWindowTitle(std::string windowTitle_arg);
	void setWindowSize(int width_arg, int height_arg);
	void setVerticalSync(bool vsync_arg);
	void setFullscreen(bool fullscreen_arg);
	void setMouseDisplay(bool mouseDisplay_arg);
	void resetMousePosition();

	void getWindowSize(int *width_arg, int *height_arg);
	void testWindowSize();
}

#endif