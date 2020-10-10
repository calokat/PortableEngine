#pragma once
#include <string>
#include "IInputSystem.h"
#include <glm/glm.hpp>
class IPlatform
{
public:
	virtual int InitWindow() = 0;
	virtual long Run() = 0;
	virtual std::string GetAssetPath(std::string relativePath) = 0;
	virtual std::wstring GetAssetPath_Wide(std::wstring relativePath) = 0;
	virtual void* GetWindowHandle() = 0;
	virtual void* GetDeviceContext() = 0;
	virtual IInputSystem* GetInputSystem() = 0;
	virtual ~IPlatform() {};
};

