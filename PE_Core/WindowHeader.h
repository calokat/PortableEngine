#pragma once
#include <entt/entt.hpp>
#include "IAssetManager.h"
#include "IRenderSystem.h"
class WindowHeader
{
public:
	void Render(entt::registry& registry, IAssetManager* assetManager, IRenderSystem* renderSystem);
private:
	void MakeMesh(const char* path, entt::registry& registry, IAssetManager* assetManager, IRenderSystem* renderSystem, const char* name = "GameObject");
};

