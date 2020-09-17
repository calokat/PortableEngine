#pragma once
#include "IPlatform.h"
#include "GameWindow.h"
#include "IGraphicsAPI.h"
#include <entt.hpp>
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

