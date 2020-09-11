#pragma once
#include "IGraphicsAPI.h"
#include "GameWindow.h"
#ifdef _WIN64
#include "WinOpenGLContext.h"
#endif
class OpenGLAPI : public IGraphicsAPI
{
public:
	int Init();
	void ClearScreen();
	void Draw();
	OpenGLAPI(GameWindow*);
private:
	GameWindow* window;
#ifdef _WIN64
	WinOpenGLContext* winGLContext;
#endif
};

