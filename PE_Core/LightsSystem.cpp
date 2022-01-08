#include "LightsSystem.h"

namespace LightsSystem
{
	void LightsSystem::ExtractLightsFromRegistry(entt::registry& reg, DirectionalLight& dirLight, PointLight* pointLights)
	{
		const auto dirLightView = reg.view<DirectionalLight>();
		dirLight = dirLightView.get<DirectionalLight>(dirLightView.front());

		auto pointLightView = reg.view<PointLight>();
		int i = 0;
		for (auto it = pointLightView.begin(); it != pointLightView.end() && i < MAX_POINT_LIGHTS; ++it, ++i)
		{
			pointLights[i] = pointLightView.get<PointLight>(*it);
		}
	}
}