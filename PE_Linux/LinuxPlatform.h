#pragma once
#include "IPlatform.h"
#include "GameWindow.h"
#include <EGL/egl.h>
#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>
#include <SDL2/SDL.h>

class LinuxPlatform :
	public IPlatform
{
public:
	LinuxPlatform(GameWindow* win);
	~LinuxPlatform();
	GameWindow* window;
	int InitWindow();
	long Run();
	std::string GetAssetPath(std::string relativePath);
	std::wstring GetAssetPath_Wide(std::wstring relativePath);
	void* GetWindowHandle();
	void* GetDeviceContext();
	IInputSystem* GetInputSystem();
	void NewGuiFrame();
	IAssetManager* GetAssetManager();
	void SetWindowResizeCallback(entt::delegate<void()> callback);
private:
	IInputSystem* inputSystem;
	IAssetManager* assetManager;
	SDL_Window* g_Window = NULL;
	entt::delegate<void()> windowResizeCallback;
};
