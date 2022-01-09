#pragma once
#include "lights.h"
#include <entt.hpp>
#include "MeshCreateInfo.h"
#include "Tree.h"
#include "IRenderSystem.h"
#include "IAssetManager.h"
namespace LightsSystem
{
	void ExtractLightsFromRegistry(entt::registry& reg, DirectionalLight& dirLight, PointLight* pointLights);
	entt::entity CreatePointLight(entt::registry& reg, Tree<MeshCreateInfo> billBoardCreateInfo, IRenderSystem* renderSystem, IAssetManager* assetManager);
	void LoadPointLightPositions(entt::basic_view<entt::entity, entt::exclude_t<>, PointLight, Transform> pointLights);
}