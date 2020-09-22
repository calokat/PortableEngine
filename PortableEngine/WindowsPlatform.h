#pragma once
#include "IPlatform.h"
#include "GameWindow.h"
#include <Windows.h>
#include <functional>
class WindowsPlatform :
	public IPlatform
{
public:
	WindowsPlatform(GameWindow* win);
	int InitWindow();
	unsigned int GetWindowWidth();
	unsigned int GetWindowHeight();
	HWND GetHwnd();
	GameWindow* window;
	long Run();
private:
	static WindowsPlatform* staticThis;
	HINSTANCE hInstance;
	//HWND hWnd;
	//unsigned int windowWidth;
	//unsigned int windowHeight;
	static LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool hasFocus;
};

