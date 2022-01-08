#pragma once
#include "lights.h"
#include <entt.hpp>

namespace LightsSystem
{
	void ExtractLightsFromRegistry(entt::registry& reg, DirectionalLight& dirLight, PointLight* pointLights);
}