#pragma once
#include "IInputSystem.h"
#include <entt.hpp>
#include <map>
#include <functional>
#include <Windows.h>
class WindowsInputSystem :
	public IInputSystem
{
public:
	WindowsInputSystem(HWND h);
	void GetKeyPressed();
	void RegisterKeyPressFunction(char, std::function<void()>);
	void RegisterRightMouseFunction(std::function<void()>);
	glm::vec2 GetCursorPosition();

private:
	std::map<char, std::function<void()>> keyToFunction;
	std::function<void()> rightClickFunction;
	HWND hwnd;
};

