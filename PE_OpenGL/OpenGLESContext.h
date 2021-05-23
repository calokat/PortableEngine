#ifdef __EMSCRIPTEN__
#pragma once
//#include "esUtil.h"
#include "GameWindow.h"
#include "IOpenGLContext.h"
#include "IPlatform.h"
#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengles2.h>


class OpenGLESContext : public IOpenGLContext
{
public:
	void* GetContext(GameWindow* window);
	void _SwapBuffers(int winHandle);
	OpenGLESContext(IPlatform* plat);
private:
	IPlatform* platform;
	// EGLDisplay eglDisplay;
	// EGLSurface eglSurface;
	SDL_GLContext g_GLContext = NULL;

};
#endif