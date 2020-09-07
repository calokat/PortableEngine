#pragma once
#include "IPlatform.h";
#include "GameWindow.h"
#include "IGraphicsAPI.h"
class PortableGame
{
public:
	PortableGame(IPlatform*, IGraphicsAPI*, GameWindow*);
	void Start();
	int Run();
private:
	IPlatform* platform;
	IGraphicsAPI* graphics;
	GameWindow* window;
};

