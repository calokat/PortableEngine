#pragma once
#include "lights.h"
#include <entt/entt.hpp>
#include "MeshCreateInfo.h"
#include "Tree.h"
#include "IRenderSystem.h"
#include "IAssetManager.h"
namespace LightsSystem
{
	void ExtractLightsFromRegistry(entt::registry& reg, DirectionalLight* dirLights, PointLight* pointLights);
	entt::entity CreatePointLight(entt::registry& reg);
	entt::entity CreateDirectionalLight(entt::registry& reg);
	void LoadPointLightPositions(entt::basic_view<entt::entity, entt::get_t<PointLight, Transform>, entt::exclude_t<>, void> pointLights);
	void LoadDirLightDirections(entt::basic_view<entt::entity, entt::get_t<DirectionalLight, Transform>, entt::exclude_t<>, void> dirLights);
}