#pragma once
#include "esUtil.h"
#include "GameWindow.h"
class OpenGLESContext
{
public:
	int GetContext(GameWindow* window);
	void SwapBuffers(int winHandle);
};
