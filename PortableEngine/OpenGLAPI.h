#pragma once
#include "IGraphicsAPI.h"
#include "GameWindow.h"
class OpenGLAPI : public IGraphicsAPI
{
public:
	int Init();
	void ClearScreen();
	void Draw();
	OpenGLAPI(GameWindow*);
private:
	GameWindow* window;
};

