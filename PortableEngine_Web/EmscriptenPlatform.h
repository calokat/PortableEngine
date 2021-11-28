#ifdef __EMSCRIPTEN__
#pragma once
#include "IPlatform.h"
#include "GameWindow.h"
#include <EGL/egl.h>
#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>
#include <emscripten/html5.h>
#include <SDL/SDL.h>

class EmscriptenPlatform :
	public IPlatform
{
public:
	EmscriptenPlatform(GameWindow* win);
	~EmscriptenPlatform();
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
	static EmscriptenPlatform* staticThis;
	static EM_BOOL BrowserWindowResizeCallback(int eventType, const EmscriptenUiEvent* uiEvent, void* userData);
	EGLNativeWindowType hWnd;
	Display *x_display;
	IInputSystem* inputSystem;
	IAssetManager* assetManager;
	SDL_Window* g_Window = NULL;
	entt::delegate<void()> windowResizeCallback;
};
#endif