#ifdef _WIN64
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
	glm::vec2 GetPreviousCursorPosition();

private:
	std::map<char, std::function<void()>> keyToFunction;
	std::function<void()> rightClickFunction;
	HWND hwnd;
	glm::vec2 cursorPos;
	glm::vec2 prevCursorPos;
};
#endif