#ifdef __EMSCRIPTEN__
#pragma once
#include "esUtil.h"
#include "GameWindow.h"
#include "IOpenGLContext.h"
#include <emscripten.h>
#include <emscripten/html5.h>

class OpenGLESContext : public IOpenGLContext
{
public:
	int GetContext(GameWindow* window);
	void _SwapBuffers(int winHandle);
};
#endif