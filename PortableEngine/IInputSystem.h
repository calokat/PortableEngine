#pragma once
#include <functional>
#include <glm/glm.hpp>
class IInputSystem
{
public:
	virtual void GetKeyPressed() = 0;
	virtual void RegisterKeyPressFunction(char, std::function<void()>) = 0;
	virtual void RegisterRightMouseFunction(std::function<void()>) = 0;
	virtual glm::vec2 GetCursorPosition() = 0;
	virtual glm::vec2 GetPreviousCursorPosition() = 0;
};