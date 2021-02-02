#ifdef _WIN64
#pragma once
#include "IPlatform.h"
#include "GameWindow.h"
#include <Windows.h>
class WindowsPlatform :
	public IPlatform
{
public:
	WindowsPlatform(GameWindow* win);
	~WindowsPlatform();
	int InitWindow();
	unsigned int GetWindowWidth();
	unsigned int GetWindowHeight();
	HWND GetHwnd();
	GameWindow* window;
	long Run();
	void* GetWindowHandle();
	void* GetDeviceContext();
	IInputSystem* GetInputSystem();
	void NewGuiFrame();
	IAssetManager* GetAssetManager();

private:
	static WindowsPlatform* staticThis;
	HINSTANCE hInstance;
	//HWND hWnd;
	//unsigned int windowWidth;
	//unsigned int windowHeight;
	static LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool hasFocus;
	HWND hwnd;
	HDC hdc;
	IInputSystem* inputSystem;
	IAssetManager* assetManager;

};
#endif
