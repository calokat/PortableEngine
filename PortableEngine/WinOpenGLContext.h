#pragma once
#include "IOpenGLContext.h"
#include "GameWindow.h"
#include <GL/glew.h>
#include <GL/wglew.h>
class WinOpenGLContext :
	public IOpenGLContext
{
public:
	int GetContext(GameWindow*);
	void _SwapBuffers(int windowHandle);
};

