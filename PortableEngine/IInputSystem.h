#pragma once
#include <functional>
class IInputSystem
{
public:
	virtual void GetKeyPressed() = 0;
	virtual void RegisterKeyPressFunction(char, std::function<void()>) = 0;
	virtual void RegisterRightMouseFunction(std::function<void()>) = 0;
};