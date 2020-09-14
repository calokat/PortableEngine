#include "PortableGame.h"

PortableGame::PortableGame(IPlatform* plat, IGraphicsAPI* graph, GameWindow* win) : platform(plat), graphics(graph), window(win)
{
}

void PortableGame::Start()
{
	platform->InitWindow();
	graphics->Init();
}

int PortableGame::Run()
{
	// while (platform->Run() == 0)
	// {
		graphics->ClearScreen();
		graphics->Draw();
	// }

	return 0;
}
