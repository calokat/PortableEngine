#ifdef _WIN64
#pragma once
#include "IInputSystem.h"
#include <entt.hpp>
#include <vector>
#include <functional>
#include <Windows.h>


struct InputFunction 
{
	std::function<void()> func;
	char key;
};

class WindowsInputSystem :
	public IInputSystem
{
public:
	WindowsInputSystem(HWND h);
	void GetKeyPressed();
	glm::vec2 GetCursorPosition();
	glm::vec2 GetPreviousCursorPosition();
	bool IsKeyPressed(KeyCode kc);
	bool WasKeyPressed(KeyCode kc);
	bool IsMouseButtonPressed(MouseButton mb);
	bool WasMouseButtonPressed(MouseButton mb);

private:
	HWND hwnd;
};
#endif