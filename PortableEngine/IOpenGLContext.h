#pragma once
#include "GameWindow.h"
class IOpenGLContext
{
public:
	virtual void* GetContext(GameWindow*) = 0;
	virtual void _SwapBuffers(int winHandle) = 0;
	virtual ~IOpenGLContext(){};
};

