#pragma once
#include "GameWindow.h"
class IOpenGLContext
{
public:
	virtual int GetContext(GameWindow*) = 0;
	virtual void _SwapBuffers(int winHandle) = 0;
	virtual ~IOpenGLContext(){};
};

