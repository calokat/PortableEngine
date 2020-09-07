#pragma once
struct GameWindow
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	int windowHandle;
	int deviceContext;
	GameWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
};

