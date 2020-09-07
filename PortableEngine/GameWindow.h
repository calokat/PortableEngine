#pragma once
struct GameWindow
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	int windowHandle;
	GameWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height, int whnd);
};

