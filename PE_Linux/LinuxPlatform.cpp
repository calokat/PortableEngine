#include "LinuxPlatform.h"
#include "LinuxInputSystem.h"
#include <GL/glew.h>
#include "LinuxAssetManager.h"
#include <imgui_impl_sdl.h>

LinuxPlatform::LinuxPlatform(GameWindow* win) : window(win)
{
	inputSystem = new LinuxInputSystem();
	assetManager = new LinuxAssetManager();
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return;
	}
}

LinuxPlatform::~LinuxPlatform()
{
	ImGui_ImplSDL2_Shutdown();
	delete inputSystem;
	delete assetManager;
}

int LinuxPlatform::InitWindow()
{
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);
	g_Window = SDL_CreateWindow("Portable Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window->width, window->height, window_flags);

	SDL_RaiseWindow(g_Window);
	return 0;
}

long LinuxPlatform::Run()
{
	return 0;
}

std::string LinuxPlatform::GetAssetPath(std::string relativePath)
{
	return relativePath;
}

std::wstring LinuxPlatform::GetAssetPath_Wide(std::wstring relativePath)
{
	return relativePath;
}
void* LinuxPlatform::GetWindowHandle()
{
	return g_Window;
}
void* LinuxPlatform::GetDeviceContext()
{
	return g_Window;
}
IInputSystem* LinuxPlatform::GetInputSystem()
{
	return inputSystem;
}
void LinuxPlatform::NewGuiFrame()
{
	ImGui_ImplSDL2_NewFrame(g_Window);
}

IAssetManager* LinuxPlatform::GetAssetManager()
{
	return assetManager;
}

void LinuxPlatform::SetWindowResizeCallback(entt::delegate<void()> callback)
{
	windowResizeCallback = callback;
}