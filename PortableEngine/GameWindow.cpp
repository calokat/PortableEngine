#include "GameWindow.h"

GameWindow::GameWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height) : x(x), y(y), width(width), height(height)
{
	windowHandle = 0;
	deviceContext = 0;
}
