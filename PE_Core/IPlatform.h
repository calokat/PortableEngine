#pragma once
#include <string>
#include "IInputSystem.h"
#include <glm/glm.hpp>
#include "IAssetManager.h"
#include <entt.hpp>
#ifndef __EMSCRIPTEN__
#include "../PE_XR/IXRPlatformPlugin.h"
#endif
class IPlatform
{
public:
	virtual int InitWindow() = 0;
	virtual long Run() = 0;
	virtual void* GetWindowHandle() = 0;
	virtual void* GetDeviceContext() = 0;
	virtual IInputSystem* GetInputSystem() = 0;
	virtual void NewGuiFrame() = 0;
	virtual IAssetManager* GetAssetManager() = 0;
	virtual void SetWindowResizeCallback(entt::delegate<void()> callback) = 0;
#ifndef __EMSCRIPTEN__
	virtual IXRPlatformPlugin* GetXRPlatformPlugin() = 0;
#endif
	virtual ~IPlatform() {};
};

