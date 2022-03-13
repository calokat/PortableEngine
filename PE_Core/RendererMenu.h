#pragma once
#include <entt/entt.hpp>
#include "IRenderSystem.h"
#include "misc_components.h"

class RendererMenu
{
public:
	void Render(entt::registry& registry, entt::entity selected, IRenderSystem* renderSystem);
};

